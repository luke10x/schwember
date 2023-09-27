#pragma once

#include "gl.h"
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "ctx_core.h"
#include "ctx_init.h"

#include "buffers.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "mesh.h"
#include "mesh_samples.h"
#include "model.h"
#include "model_load.h"
#include "physics.h"

/*
** Context is the initialization data, and data that
** has to persist between frames
*/
typedef struct {
  int should_continue;
  GLFWwindow* window;
  shader_t* default_shader;
  shader_t* light_shader;
  shader_t* sky_shader;

  physics_t* physics;

  mesh_t*   pyramid;
  glm::mat4 pyramid_transform;
  btRigidBody* pyramid_body__;

  mesh_t*   myramid;
  glm::mat4 myramid_transform;
  btRigidBody* myramid_body__;
  glm::vec3 pbbc__;

  mesh_t* floor;
  glm::mat4 floor_transform;

  mesh_t* lamp;

  model_t* test_model;

  model_t* map_model;

  model_t* skybox;

  model_t* sign;
  glm::mat4 sign_transform;

  model_t* subject;
  glm::mat4 subject_transform;

  model_t* suzanne;
  glm::mat4 suzanne_transform;

  camera_t* camera;
} ctx_t;

void ctx_load(ctx_t* ctx, int width, int height) {
  // Setup shaders
  ctx->default_shader = shader_create("src/shaders/default.vert", "src/shaders/default.frag");
  ctx->light_shader   = shader_create("src/shaders/light.vert", "src/shaders/light.frag");
  ctx->sky_shader     = shader_create("src/shaders/default.vert", "src/shaders/sky.frag");

  // light source
  glm::vec4 light_color = glm::vec4(1.0f, 0.9f, 0.7f, 0.5f);
  glm::vec3 light_pos   = glm::vec3(-3.0f, 2.5f, 0.0f); // 03:00

  // all shaders use the light source (for reflections)
  shader_set_uniform_vec4(ctx->light_shader, "lightColor", light_color);
  shader_set_uniform_vec4(ctx->default_shader, "lightColor", light_color);
  shader_set_uniform_vec3(ctx->default_shader, "lightPos",   light_pos);
  shader_set_uniform_vec4(ctx->sky_shader, "lightColor", light_color);
  shader_set_uniform_vec3(ctx->sky_shader, "lightPos",   light_pos);

  // Lamp is placed where the light source is
  glm::mat4 lamp_transform = translate(glm::mat4(1.0f), light_pos);
  shader_set_uniform_mat4(ctx->light_shader, "modelToWorld", lamp_transform);
  ctx->lamp = mesh_sample_create_lamp();

  // Pyramid
  ctx->pyramid = mesh_sample_create_pyramid();
  ctx->pyramid_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 4, 2));
  ctx->pyramid_transform = glm::scale(ctx->pyramid_transform, glm::vec3(2.0f, 2.0f, 2.0f));

  // Myramid
  ctx->myramid = mesh_sample_create_pyramid();
  ctx->myramid_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-3.1, 7, 2));
  ctx->myramid_transform = glm::scale(ctx->myramid_transform, glm::vec3(2.0f, 2.0f, 2.0f));


  // Just in the middle
  ctx->floor = mesh_sample_create_floor();
  ctx->floor_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.01, 0)); // one cm up

  glm::mat4 skybox_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, -0.5, 0.0f));
  shader_set_uniform_mat4(ctx->sky_shader, "modelToWorld", skybox_transform);
  ctx->skybox = model_create();
  model_load_from_file(ctx->skybox, "assets/gltf/skybox.glb");

  ctx->sign = model_create();
  model_load_from_file(ctx->sign, "assets/gltf/pixel-perfect-sign.glb");
  ctx->sign_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-6, 0, -2));

  ctx->test_model = model_create();
  model_load_from_file(ctx->test_model, "assets/gltf/texture-test.glb");
  
  ctx->map_model = model_create();
  model_load_from_file(ctx->map_model, "assets/gltf/noob-level-map.glb");
  
  ctx->subject = model_create();
  model_load_from_file(ctx->subject, "assets/gltf/noob.glb");
  ctx->subject_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-6, 0, 1));

  ctx->suzanne = model_create();
  model_load_from_file(ctx->suzanne, "assets/gltf/suzanne.glb");
  ctx->suzanne_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 4, 1));

  ctx->camera = camera_create(width, height,
    glm::vec3(-glm::sqrt(3), 1.8f, -3),  // South-East side: 05:00
    glm::vec3(ctx->pyramid_transform[3]) // Look at the pyramid
  );

  // We use physics
  ctx->physics = physics_create();
  
  // Floor physics
  btVector3 planeNormal(0, 1, 0);
  btScalar planeConstant = 0.0;
  // Create a btStaticPlaneShape
  btCollisionShape* floor_collision_shape = new btStaticPlaneShape(
    planeNormal,
    planeConstant
  ); 
  ctx->physics->collision_shapes.push_back(floor_collision_shape);
  btTransform floor_bt_transform;
  // Use the glm matrix's first column
  // which contains the rotation and scale components
  floor_bt_transform.setFromOpenGLMatrix(glm::value_ptr(ctx->floor_transform));
  btScalar floor_mass(0.0);
	btVector3 floor_local_inertia(0, 0, 0);
	btDefaultMotionState* floor_motion_state = new btDefaultMotionState(floor_bt_transform);
	btRigidBody::btRigidBodyConstructionInfo floor_rb_info(
    floor_mass,
    floor_motion_state,
    floor_collision_shape,
    floor_local_inertia
  );
	btRigidBody* floor_body = new btRigidBody(floor_rb_info);
	ctx->physics->dynamics_world->addRigidBody(floor_body);

  // Pyramid physics
  glm::vec3 pbb = mesh_calculate_bounding_box(ctx->pyramid);
  printf("Pyramid BOUNDiNG: %f, %f, %f\n", pbb.x, pbb.y, pbb.z);
  ctx->pbbc__ = mesh_calculate_center_shift(ctx->pyramid);
    printf("Pyramid Bounding Center shift: %f, %f, %f\n",
    ctx->pbbc__.x,
    ctx->pbbc__.y,
    ctx->pbbc__.z
  );
  btCollisionShape* pyramid_collision_shape = new btBoxShape(
    btVector3(
      btScalar(pbb.x),
      btScalar(pbb.y),
      btScalar(pbb.z)
    )
  );
  ctx->physics->collision_shapes.push_back(pyramid_collision_shape);
  btTransform pyramid_bt_transform;
  // Use the glm matrix's first column
  // which contains the rotation and scale components
  pyramid_bt_transform.setFromOpenGLMatrix(glm::value_ptr(ctx->pyramid_transform));
  btScalar pyramid_mass(1.0);
	btVector3 pyramid_local_inertia(0, 0, 0);
  // Dynamic is this Pyramid
	pyramid_collision_shape->calculateLocalInertia(
    pyramid_mass, pyramid_local_inertia
  );
  btDefaultMotionState* pyramid_motion_state = new btDefaultMotionState(
    pyramid_bt_transform 
  );
	btRigidBody::btRigidBodyConstructionInfo pyramid_rb_info(
    pyramid_mass,
    pyramid_motion_state,
    pyramid_collision_shape,
    pyramid_local_inertia
  );
	ctx->pyramid_body__ = new btRigidBody(pyramid_rb_info);
	ctx->physics->dynamics_world->addRigidBody(ctx->pyramid_body__);

  // Myramid physics
  glm::vec3 mbb = mesh_calculate_bounding_box(ctx->pyramid);
  printf("Myramid BOUNDiNG: %f, %f, %f\n", mbb.x, mbb.y, mbb.z);
  glm::vec3 mbbc =mesh_calculate_center_shift(ctx->pyramid);
  printf("Myramid Center shift: %f, %f, %f\n", mbbc.x, mbbc.y, mbbc.z);
  btCollisionShape* myramid_collision_shape = new btBoxShape(
    btVector3(
      btScalar(mbb.x),
      btScalar(mbb.y),
      btScalar(mbb.z)
    )
  );
  ctx->physics->collision_shapes.push_back(myramid_collision_shape);
  btTransform myramid_bt_transform;
  myramid_bt_transform.setFromOpenGLMatrix(glm::value_ptr(
    ctx->myramid_transform
  ));
  btScalar myramid_mass(1.0);
	btVector3 myramid_local_inertia(0, 0, 0);
  // Dynamic collision shapes need inertia
	myramid_collision_shape->calculateLocalInertia(
    myramid_mass, myramid_local_inertia
  );
  btDefaultMotionState* myramid_motion_state = new btDefaultMotionState(
    myramid_bt_transform
  );
	btRigidBody::btRigidBodyConstructionInfo myramid_rb_info(
    myramid_mass,
    myramid_motion_state,
    myramid_collision_shape,
    myramid_local_inertia
  );
	ctx->myramid_body__ = new btRigidBody(myramid_rb_info);
	ctx->physics->dynamics_world->addRigidBody(ctx->myramid_body__);
}

/**
 * Render frame
 */
inline static void ctx_render(ctx_t* ctx) {
  int fps = 100; // TODO use real (or desired) value
  float time_step = 1.0f / fps;
  physics_step_simulation(ctx->physics, time_step);

  // Pyramid render
	btTransform trans;
  ctx->pyramid_body__->getMotionState()->getWorldTransform(trans);
  // printf("TARNS: %f, %f, %f \n",
  //   float(trans.getOrigin().getX()),
  //   float(trans.getOrigin().getY()),
  //   float(trans.getOrigin().getZ())
  // );
  trans.getOpenGLMatrix(glm::value_ptr(ctx->pyramid_transform));
  // Compensation for collision shape center offet
  ctx->pyramid_transform = glm::translate(
    ctx->pyramid_transform,
    glm::vec3(0.0f, -0.8f, 0.0f)
  );
  ctx->pyramid_transform = glm::scale(ctx->pyramid_transform, glm::vec3(2.0f, 2.0f, 2.0f));
  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->pyramid_transform);
  mesh_draw(ctx->pyramid, ctx->default_shader, ctx->camera);

  // Myramid render
	btTransform m_trans;
  ctx->myramid_body__->getMotionState()->getWorldTransform(m_trans);
  m_trans.getOpenGLMatrix(glm::value_ptr(ctx->myramid_transform));
  ctx->myramid_transform = glm::scale(ctx->myramid_transform, glm::vec3(2.0f, 2.0f, 2.0f));
  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->myramid_transform);
  mesh_draw(ctx->myramid, ctx->default_shader, ctx->camera);

  // Floor render
  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->floor_transform);
  mesh_draw(ctx->floor, ctx->default_shader, ctx->camera);

  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", glm::mat4(1.0));

  model_draw(ctx->test_model, ctx->default_shader, ctx->camera);
  model_draw(ctx->map_model, ctx->default_shader, ctx->camera);
  model_draw(ctx->skybox, ctx->sky_shader, ctx->camera);

  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->sign_transform);
  model_draw(ctx->sign, ctx->default_shader, ctx->camera);

  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->subject_transform);
  model_draw(ctx->subject, ctx->default_shader, ctx->camera);
  
  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->suzanne_transform);
  model_draw(ctx->suzanne, ctx->default_shader, ctx->camera);

  // transparent must render last
  mesh_draw(ctx->lamp, ctx->light_shader, ctx->camera);

  // ImGui render
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
          // Your ImGui code here
  ImGui::Begin("Hello, world!");
  ImGui::Text("This is a simple ImGui application.");
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

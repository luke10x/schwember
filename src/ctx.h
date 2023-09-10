#pragma once

#include "gl.h"
#include <GLFW/glfw3.h>

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

  // Pyramid in the north
  ctx->pyramid = mesh_sample_create_pyramid();
  ctx->pyramid_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 4, 2));
  // inflate it a bit so that it matches floor texture
  ctx->pyramid_transform = glm::scale(ctx->pyramid_transform, glm::vec3(2.0f, 2.0f, 2.0f));

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

  ctx->camera = camera_create(width, height,
    glm::vec3(-glm::sqrt(3), 1.8f, -3),  // South-East side: 05:00
    glm::vec3(ctx->pyramid_transform[3]) // Look at the pyramid
  );

  ctx->physics = physics_create();
  
  // Floor physics
  
  glm::vec3 bb = mesh_calculate_bounding_box(ctx->floor);
  printf("BOUNDiNG: %f, %f, %f\n", bb.x, bb.y, bb.z);

  btCollisionShape* floor_collision_shape = new btBoxShape(
    btVector3(
      btScalar(bb.x / 2.0),
      btScalar(bb.y / 2.0),
      btScalar(bb.z / 2.0)
    )
  );

printf("after floor_collision_shape\n");

  ctx->physics->collision_shapes.push_back(floor_collision_shape);
printf("after floor_collision_shape pushed back\n");
	
  btTransform floor_bt_transform;
  // Use the glm matrix's first column
  // which contains the rotation and scale components
  floor_bt_transform.setFromOpenGLMatrix(glm::value_ptr(ctx->floor_transform[0]));
printf("after floor_bt_transform set\n");
	
  btScalar floor_mass(0.0);
	btVector3 floor_local_inertia(0, 0, 0);
printf("after mass and inertia created\n");

	btDefaultMotionState* floor_motion_state = new btDefaultMotionState(floor_bt_transform);
printf("after floor_motion_state created\n");

	btRigidBody::btRigidBodyConstructionInfo floor_rb_info(
    floor_mass,
    floor_motion_state,
    floor_collision_shape,
    floor_local_inertia
  );
	btRigidBody* floor_body = new btRigidBody(floor_rb_info);
printf("after floor_body created\n");

	ctx->physics->dynamics_world->addRigidBody(floor_body);
printf("after floor_body added to dynamics_world\n");

  // Pyramid physics
  glm::vec3 pbb = mesh_calculate_bounding_box(ctx->pyramid);
  printf("Pyramid BOUNDiNG: %f, %f, %f\n", pbb.x, pbb.y, pbb.z);


  btCollisionShape* pyramid_collision_shape = new btBoxShape(
    btVector3(
      btScalar(pbb.x / 1.0),
      btScalar(pbb.y / 1.0),
      btScalar(pbb.z / 1.0)
    )
  );

  ctx->physics->collision_shapes.push_back(pyramid_collision_shape);
	
  btTransform pyramid_bt_transform;
  // Use the glm matrix's first column
  // which contains the rotation and scale components
  pyramid_bt_transform.setFromOpenGLMatrix(glm::value_ptr(ctx->pyramid_transform[0]));
	
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
}


inline static void ctx_render(ctx_t* ctx) {

  int fps = 60; // TODO use real value
  float time_step = 1.0f / fps;
  physics_step_simulation(ctx->physics, time_step);

	btTransform trans;

  ctx->pyramid_body__->getMotionState()->getWorldTransform(trans);

  // printf("TARNS: %f, %f, %f \n",
  //   float(trans.getOrigin().getX()),
  //   float(trans.getOrigin().getY()),
  //   float(trans.getOrigin().getZ())
  // );

  trans.getOpenGLMatrix(glm::value_ptr(ctx->pyramid_transform));


  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->pyramid_transform);
  mesh_draw(ctx->pyramid, ctx->default_shader, ctx->camera);

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


  // transparent must render last
  mesh_draw(ctx->lamp, ctx->light_shader, ctx->camera);
}

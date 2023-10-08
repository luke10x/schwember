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
#include "renderable.h"
#include "collider.h"
#include "pc.h"
#include "ui.h"

// *********************************************************************
// Context is the initialization data, and data that
// has to persist between frames
// *********************************************************************
typedef struct {
  int should_continue;
  GLFWwindow* window;
  shader_t* default_shader;
  shader_t* light_shader;
  shader_t* sky_shader;

  physics_t* physics;

  mesh_t*     pyramid;
  glm::mat4   pyramid_transform;
  collider_t* pyramid_collider;

  mesh_t*     myramid;
  glm::mat4   myramid_transform;
  collider_t* myramid_collider;

  mesh_t* floor;
  glm::mat4 floor_transform;

  mesh_t* lamp;

  model_t* test_model;

  model_t*  map_model;
  glm::mat4 map_transform;


  model_t* skybox;

  model_t* sign;
  glm::mat4 sign_transform;

  model_t* subject;
  glm::mat4 subject_transform;

  model_t* suzanne;
  glm::mat4 suzanne_transform;

  model_t* stickman;
  glm::mat4 stickman_transform;

  model_t*    sphere;
  glm::mat4   sphere_transform;
  collider_t* sphere_collider;

  camera_t* camera;

  pc_t* pc;

  ui_t* ui;
} ctx_t;

// *********************************************************************
// Collider Contact CallBack
// TODO: this should not be here
// It belongs to physics system
// *********************************************************************
class MyCcb : public btCollisionWorld::ContactResultCallback {
private:
  int counter = 0;
public:
  virtual btScalar addSingleResult(
    btManifoldPoint& cp,
    const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
    const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1
  ) {
    counter++;
    if (counter % 100 == 0) {
      btScalar distance = cp.getDistance();
      
      float x = cp.m_positionWorldOnA.getX();
      float y = cp.m_positionWorldOnA.getY();
      float z = cp.m_positionWorldOnA.getZ();
    
      // printf("🙈 🙈 🙈 100 detected! (%.2f, %.2f, %.2f) dist=%.2f\n", x, y, z, distance);
    }

    return btScalar(1);
  }
};
MyCcb my_ccb;

// *********************************************************************
// Load context
// ============
// - Initialize everything
// - Executed at start
// *********************************************************************
void ctx_load(ctx_t* ctx, int width, int height) {

  // We use physics
  ctx->physics = physics_create();

  // Setup shaders
  ctx->default_shader = shader_create(
    "src/shaders/default.vert",
    "src/shaders/default.frag"
  );
  ctx->light_shader = shader_create(
    "src/shaders/light.vert",
    "src/shaders/light.frag"
  );
  ctx->sky_shader = shader_create(
    "src/shaders/default.vert",
    "src/shaders/sky.frag"
  );

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
  ctx->pyramid_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 4, 4));
  ctx->pyramid_transform = glm::scale(
    ctx->pyramid_transform,
    glm::vec3(1.5f, 1.5f, 1.5f)
  );
  ctx->pyramid_collider = collider_create_box_from_mesh(
    ctx->pyramid_transform,
    ctx->pyramid,
    ctx->physics
  );

  // Myramid
  ctx->myramid = mesh_sample_create_pyramid();
  ctx->myramid_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-3.1, 7, 4));
  ctx->myramid_transform = glm::scale(ctx->myramid_transform, glm::vec3(3.0f, 3.0f, 3.0f));
  ctx->myramid_collider = collider_create_box_from_mesh(
    ctx->myramid_transform,
    ctx->myramid,
    ctx->physics
  );

  // Floor is in the middle
  ctx->floor = mesh_sample_create_floor();
  ctx->floor_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.01f, 0)); // one cm up
  // collider_t* floor_collider = collider_create_plane(ctx->floor_transform, ctx->physics);

  glm::mat4 skybox_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, -0.5, 0.0f));
  shader_set_uniform_mat4(ctx->sky_shader, "modelToWorld", skybox_transform);
  ctx->skybox = model_create();
  model_load_from_file(ctx->skybox, "assets/gltf/skybox.glb");

  ctx->sign = model_create();
  model_load_from_file(ctx->sign, "assets/gltf/pixel-perfect-sign.glb");
  ctx->sign_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-6, 0, -2));

  ctx->test_model = model_create();
  model_load_from_file(ctx->test_model, "assets/gltf/texture-test.glb");
  
  // Map
  ctx->map_model = model_create();
  model_load_from_file(ctx->map_model, "assets/gltf/noob-level-map.glb");
  ctx->map_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

  for (int i = 0; i < ctx->map_model->num_meshes; i++) {
    mesh_t* mesh = ctx->map_model->meshes[i];
    collider_t* map_collider = collider_create_mesh(
      ctx->map_transform, mesh, ctx->physics
    );
    ((collider_sphere_t*) map_collider)->rigid_body
      ->setRestitution(0.2);

    //    ((collider_sphere_t*) map_collider)->rigid_body
    // ->setDamping(0.0001f,0.0001f );
  }

  ctx->subject = model_create();
  model_load_from_file(ctx->subject, "assets/gltf/noob.glb");
  ctx->subject_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-6, 0, 1));

  ctx->suzanne = model_create();
  model_load_from_file(ctx->suzanne, "assets/gltf/suzanne.glb");
  ctx->suzanne_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 4, 1));

  ctx->stickman = model_create();
  model_load_from_file(ctx->stickman, "assets/gltf/ultra_low_poly_animated_character_mixamo_based.glb");
  ctx->stickman_transform = glm::translate(glm::mat4(1.0f), glm::vec3(2, 4.0, -24));

  // The main controllable player entity
  ctx->pc = pc_create(ctx->default_shader);


  ctx->stickman = model_create();
  model_load_from_file(ctx->stickman, "assets/gltf/ultra_low_poly_animated_character_mixamo_based.glb");
  ctx->stickman_transform = glm::translate(glm::mat4(1.0f), glm::vec3(2, 4.0, -24));


  // Sphere
  ctx->sphere = model_create();
  model_load_from_file(ctx->sphere, "assets/gltf/sphere.glb");
  ctx->sphere_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 8, 20-20));
  ctx->sphere_transform = glm::scale(ctx->sphere_transform, glm::vec3(0.5f, 0.5f, 0.5f));
  ctx->sphere_collider = collider_create_sphere(
    ctx->sphere_transform,
    1.0f,
    ctx->physics
  );
  btVector3 force(-2.5f, 0.2f, -0.4f);
  ((collider_sphere_t*) ctx->sphere_collider)->rigid_body
    ->applyCentralImpulse(force);
  ((collider_sphere_t*) ctx->sphere_collider)->rigid_body
    ->setRestitution(0.8);

  // Camera 
  ctx->camera = camera_create(width, height,
    glm::vec3(-glm::sqrt(3), 1.8f, -3),  // South-East side: 05:00
    glm::vec3(ctx->pyramid_transform[3]) // Look at the pyramid
  );

  ctx->ui = ui_create();
}

// *********************************************************************
// Render frame
// ============
// Executes on every frame
// *********************************************************************
inline static void ctx_render(ctx_t* ctx) {
  int fps = 100; // TODO use real (or desired) value
  float time_step = 1.0f / fps;
  physics_step_simulation(ctx->physics, time_step);

  ctx->physics->dynamics_world
    ->contactTest(
      ((collider_sphere_t*) ctx->sphere_collider)->rigid_body,
      my_ccb
  );

  // Pyramid render
  ctx->pyramid_transform = collider_update_transform(
    ctx->pyramid_collider,
    ctx->pyramid_transform
  );
  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->pyramid_transform);
  renderable_draw(ctx->pyramid->renderable, ctx->default_shader, ctx->camera);

  // Myramid render
  ctx->myramid_transform = collider_update_transform(
    ctx->myramid_collider,
    ctx->myramid_transform
  );
  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->myramid_transform);
  renderable_draw(ctx->myramid->renderable, ctx->default_shader, ctx->camera);

  // Floor render
  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->floor_transform);
  renderable_draw(ctx->floor->renderable, ctx->default_shader, ctx->camera);

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

  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->stickman_transform);
  model_draw(ctx->stickman, ctx->default_shader, ctx->camera);

  ctx->sphere_transform = collider_update_transform(
    ctx->sphere_collider,
    ctx->sphere_transform
  );
  
  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->sphere_transform);
  model_draw(ctx->sphere, ctx->default_shader, ctx->camera);

  // transparent must render last
  renderable_draw(ctx->lamp->renderable, ctx->light_shader, ctx->camera);

  // main character render
  pc_draw(ctx->pc, ctx->camera);

  ui_draw(ctx->ui, ctx->pc);
}

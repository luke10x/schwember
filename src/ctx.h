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
#include "renderable.h"
#include "collider.h"

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

  model_t* map_model;

  model_t* skybox;

  model_t* sign;
  glm::mat4 sign_transform;

  model_t* subject;
  glm::mat4 subject_transform;

  model_t* suzanne;
  glm::mat4 suzanne_transform;

  model_t* stickman;
  glm::mat4 stickman_transform;

  model_t* sphere;
  glm::mat4 sphere_transform;

  camera_t* camera;
} ctx_t;

void ctx_load(ctx_t* ctx, int width, int height) {

  // We use physics
  ctx->physics = physics_create();

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
  // ctx->pyramid_transform = glm::scale(ctx->pyramid_transform, glm::vec3(2.0f, 2.0f, 2.0f));
  ctx->pyramid_collider = collider_create_box_from_mesh(
    ctx->pyramid_transform,
    ctx->pyramid,
    ctx->physics
  );

  // Myramid
  ctx->myramid = mesh_sample_create_pyramid();
  ctx->myramid_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-3.1, 7, 2));
  ctx->myramid_transform = glm::scale(ctx->myramid_transform, glm::vec3(2.0f, 2.0f, 2.0f));
  ctx->myramid_collider = collider_create_box_from_mesh(
    ctx->myramid_transform,
    ctx->myramid,
    ctx->physics
  );

  // Floor is in the middle
  ctx->floor = mesh_sample_create_floor();
  ctx->floor_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.01, 0)); // one cm up
  // Create physics body for floor
  collider_t* floor_collider = collider_create_plane(ctx->floor_transform, ctx->physics);

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

  ctx->stickman = model_create();
  model_load_from_file(ctx->stickman, "assets/gltf/ultra_low_poly_animated_character_mixamo_based.glb");
  ctx->stickman_transform = glm::translate(glm::mat4(1.0f), glm::vec3(2, 4.0, -24));

  ctx->sphere = model_create();
  model_load_from_file(ctx->sphere, "assets/gltf/sphere.glb");
  ctx->sphere_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 3, -20));

  ctx->camera = camera_create(width, height,
    glm::vec3(-glm::sqrt(3), 1.8f, -3),  // South-East side: 05:00
    glm::vec3(ctx->pyramid_transform[3]) // Look at the pyramid
  );

}

/**
 * Render frame
 */
inline static void ctx_render(ctx_t* ctx) {
  int fps = 100; // TODO use real (or desired) value
  float time_step = 1.0f / fps;
  physics_step_simulation(ctx->physics, time_step);

  // Pyramid render
  ctx->pyramid_transform = collider_update_transform(
    ctx->pyramid_collider,
    ctx->pyramid_transform
  );

///
  // ctx->pyramid_transform = glm::scale(ctx->pyramid_transform, glm::vec3(2.0f, 2.0f, 2.0f));
  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->pyramid_transform);
  renderable_draw(ctx->pyramid->renderable, ctx->default_shader, ctx->camera);

  // Myramid render
  ctx->myramid_transform = collider_update_transform(
    ctx->myramid_collider,
    ctx->myramid_transform
  );

  ctx->myramid_transform = glm::scale(ctx->myramid_transform, glm::vec3(2.0f, 2.0f, 2.0f));
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

  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->sphere_transform);
  model_draw(ctx->sphere, ctx->default_shader, ctx->camera);

  // transparent must render last
  renderable_draw(ctx->lamp->renderable, ctx->light_shader, ctx->camera);

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

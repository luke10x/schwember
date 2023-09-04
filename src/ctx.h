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

  mesh_t*   pyramid;
  glm::mat4 pyramid_transform;

  mesh_t* floor;

  mesh_t* lamp;

  model_t* test_model;

  model_t* map_model;

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
  ctx->pyramid_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 2));
  // inflate it a bit so that it matches floor texture
  ctx->pyramid_transform = glm::scale(ctx->pyramid_transform, glm::vec3(2.0f, 2.0f, 2.0f));

  // Just in the middle
  ctx->floor = mesh_sample_create_floor();

  // TODO add skybox mesh
  glm::mat4 skybox_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, -0.5, 0.0f));
  shader_set_uniform_mat4(ctx->sky_shader, "modelToWorld", skybox_transform);

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
}

inline static void ctx_render(ctx_t* ctx) {
  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->pyramid_transform);
  mesh_draw(ctx->pyramid, ctx->default_shader, ctx->camera);

  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", glm::mat4(1.0));
  mesh_draw(ctx->floor, ctx->default_shader, ctx->camera);

  model_draw(ctx->test_model, ctx->default_shader, ctx->camera);
  model_draw(ctx->map_model, ctx->default_shader, ctx->camera);

  shader_set_uniform_mat4(ctx->default_shader, "modelToWorld", ctx->subject_transform);
  model_draw(ctx->subject, ctx->default_shader, ctx->camera);

  // transparent must render last
  mesh_draw(ctx->lamp, ctx->light_shader, ctx->camera);
}

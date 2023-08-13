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
  
  camera_t* camera;
} ctx_t;

void ctx_load(ctx_t* ctx, int width, int height) {
  ctx->default_shader = shader_create("src/shaders/default.vert", "src/shaders/default.frag");
  ctx->light_shader   = shader_create("src/shaders/light.vert", "src/shaders/light.frag");
  ctx->sky_shader     = shader_create("src/shaders/default.vert", "src/shaders/sky.frag");

  glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
  glm::vec3 light_pos   = glm::vec3(1.0f, 1.0f, 0.5f);
  glm::mat4 lamp_transform = translate(glm::mat4(1.0f), light_pos);
  ctx->lamp                = mesh_sample_create_lamp();

  ctx->pyramid_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, -0.5, 0.0f));
  ctx->pyramid           = mesh_sample_create_pyramid();

  ctx->floor = mesh_sample_create_floor();

  glm::mat4 skybox_transform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5, -0.5, 0.0f));

	shader_activate(ctx->light_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->light_shader->ID, "modelToWorld"), 1, GL_FALSE, glm::value_ptr(lamp_transform));
  glUniform4f(glGetUniformLocation(ctx->light_shader->ID, "lightColor"), light_color[0], light_color[1], light_color[2], light_color[3]);

  shader_activate(ctx->default_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->default_shader->ID, "modelToWorld"), 1, GL_FALSE, glm::value_ptr(ctx->pyramid_transform));
	glUniform4f(glGetUniformLocation(ctx->default_shader->ID, "lightColor"), light_color[0], light_color[1], light_color[2], light_color[3]);
  glUniform3f(glGetUniformLocation(ctx->default_shader->ID, "lightPos"), light_pos[0], light_pos[1], light_pos[2]);

  shader_activate(ctx->sky_shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->sky_shader->ID, "modelToWorld"), 1, GL_FALSE, glm::value_ptr(skybox_transform));
	glUniform4f(glGetUniformLocation(ctx->default_shader->ID, "lightColor"), light_color[0], light_color[1], light_color[2], light_color[3]);
  glUniform3f(glGetUniformLocation(ctx->default_shader->ID, "lightPos"), light_pos[0], light_pos[1], light_pos[2]);

  ctx->camera = camera_create(width, height, glm::vec3(2.0f, 1.8f, 3.0f));
}


inline static void ctx_render(ctx_t* ctx) {
    // Specify the color of the background

    glClearColor(0.4f, 0.33f, 0.17f, 1.0f);
    // Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mesh_draw(ctx->floor,   ctx->default_shader, ctx->camera);

    shader_activate(ctx->light_shader);
    mesh_draw(ctx->lamp, ctx->light_shader, ctx->camera);

    shader_activate(ctx->default_shader);
  	glUniformMatrix4fv(glGetUniformLocation(ctx->default_shader->ID, "modelToWorld"), 1, GL_FALSE, (GLfloat*)glm::value_ptr(ctx->pyramid_transform));
    mesh_draw(ctx->pyramid, ctx->default_shader, ctx->camera);

    mesh_draw(ctx->pyramid, ctx->default_shader, ctx->camera);

    // Swap the back buffer with the front buffer
    glfwSwapBuffers(ctx->window);
}

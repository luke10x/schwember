#pragma once

#include "gl.h"
#include "texture.h"
#include "buffers.h"
#include "camera.h"

typedef struct {
  texture_t* albedo_texture;
  GLuint     vao_id;
  GLsizei    index_count;
} renderable_t;

renderable_t* renderable_create(
  vertex_t* vertices,
  GLsizei vertex_count,
  GLuint* indices,
  GLsizei index_count,
  texture_t* albedo_texture
) {
  renderable_t* self = (renderable_t*) malloc(sizeof(renderable_t));

  self->index_count = index_count;

  self->albedo_texture = NULL;
  if (albedo_texture != NULL) {
    self->albedo_texture = (texture_t*) malloc(sizeof(texture_t));
    texture_t* dest = self->albedo_texture;
    memcpy(dest, albedo_texture, sizeof(texture_t));
  }

  // Create VAO
  glGenVertexArrays(1, &(self->vao_id));
  glBindVertexArray(self->vao_id);

  // Create VBO with vertices
  GLuint vbo_id;
  glGenBuffers(1, &vbo_id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
  size_t vbo_size = vertex_count * sizeof(vertex_t);
  glBufferData(GL_ARRAY_BUFFER, vbo_size, vertices, GL_STATIC_DRAW);

  // Create EBO with indexes
  GLuint ebo_id;
  glGenBuffers(1, &ebo_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
  size_t ebo_size = self->index_count * sizeof(GLuint);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_size, indices, GL_STATIC_DRAW);  

  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

	// Links VBO attributes such as coordinates and colors to VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)0);                   // position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(3 * sizeof(float))); // color
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(6 * sizeof(float))); // tex_UV
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(8 * sizeof(float))); // tex_id
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(9 * sizeof(float))); // normal
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(12 * sizeof(float))); // 4 bones
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(16 * sizeof(float))); // 4 weights

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Unbind all to prevent accidentally modifying them
  glBindVertexArray(0);                     // VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);         // VBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // EBO

  return self;
}

void renderable_draw(renderable_t* self, shader_t* shader, camera_t* camera) {

  shader_activate(shader);

  GLint use_sampler_location = glGetUniformLocation(shader->ID, "useSampler");

  if (self->albedo_texture != NULL) {
    glUniform1i(use_sampler_location, true);
    
    // Texture unit
    GLuint location = glGetUniformLocation(shader->ID, "sampler");
    glUniform1i(location, 0);
    
    // Texture bind
    glActiveTexture(self->albedo_texture->unit);
    glBindTexture(GL_TEXTURE_2D, self->albedo_texture->ID);
  } else {
    glUniform1i(use_sampler_location, false);
  }

  // Take care of the camera Matrix
  glUniform3f(
    glGetUniformLocation(shader->ID, "camPos"),
    camera->view_matrix[3][0],
    camera->view_matrix[3][1],
    camera->view_matrix[3][2]
  );

  // Camera matrix
  GLuint location = glGetUniformLocation(shader->ID, "worldToView");
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera->camera_matrix));

  glBindVertexArray(self->vao_id);
  glDrawElements(
    GL_TRIANGLES,
    self->index_count,
    GL_UNSIGNED_INT,
    (void*) (0 * sizeof(unsigned int))
  );
}
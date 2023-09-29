#pragma once

#include <string.h>

#include <glm/glm.hpp>

#include "camera.h"
#include "texture.h"

#pragma once

typedef struct {
  char* name;
  vertex_t* vertices;
  GLsizei vertex_count;

  GLuint* indices;
  GLsizei index_count;

  texture_t* albedo_texture;

  GLuint vao_id;
} mesh_t;

mesh_t* mesh_create(
  const char* name,
  vertex_t* vertices,
  GLsizei vertex_count,
  GLuint* indices,
  GLsizei index_count,
  texture_t* albedo_texture
) {
  mesh_t* mesh = (mesh_t*) malloc(sizeof(mesh_t));

  mesh->vertices = (vertex_t*) malloc(vertex_count * sizeof(vertex_t));
  memcpy(mesh->vertices, vertices, vertex_count * sizeof(vertex_t));
  mesh->vertex_count = vertex_count;

  mesh->indices = (GLuint*) malloc(index_count * sizeof(GLuint));
  memcpy(mesh->indices, indices, index_count * sizeof(GLuint));
  mesh->index_count = index_count;

  mesh->albedo_texture = NULL;
  if (albedo_texture != NULL) {
    mesh->albedo_texture = (texture_t*) malloc(sizeof(texture_t));
    texture_t* dest = mesh->albedo_texture;
    memcpy(dest, albedo_texture, sizeof(texture_t));
  }

  // Create VAO
  glGenVertexArrays(1, &(mesh->vao_id));
  glBindVertexArray(mesh->vao_id);

  // Create VBO with vertices
  GLuint vbo_id;
  glGenBuffers(1, &vbo_id);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
  size_t vbo_size = mesh->vertex_count * sizeof(vertex_t);
  glBufferData(GL_ARRAY_BUFFER, vbo_size, vertices, GL_STATIC_DRAW);

  // Create EBO with indexes
  GLuint ebo_id;
  glGenBuffers(1, &ebo_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
  size_t ebo_size = mesh->index_count * sizeof(GLuint);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_size, mesh->indices, GL_STATIC_DRAW);  

  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

	// Links VBO attributes such as coordinates and colors to VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)0);                   // position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(3 * sizeof(float))); // color
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(6 * sizeof(float))); // tex_UV
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(8 * sizeof(float))); // tex_id
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(9 * sizeof(float))); // normal

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

  return mesh;
}

void mesh_draw(mesh_t* mesh, shader_t* shader, camera_t* camera) {

  shader_activate(shader);

  GLint use_sampler_location = glGetUniformLocation(shader->ID, "useSampler");

  if (mesh->albedo_texture != NULL) {
    glUniform1i(use_sampler_location, true);
    texture_unit(mesh->albedo_texture, shader, "sampler", 0);
    texture_bind(mesh->albedo_texture);
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

  camera_matrix(camera, shader, "worldToView");
  
  glBindVertexArray(mesh->vao_id);
  glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, (void*) (0 * sizeof(unsigned int)));
}

/**
 * Calculates bounding box vector of mesh vertices
 */
glm::vec3 mesh_calculate_bounding_box(mesh_t* self) {
    // Use first vertex as initial min and max value
    vertex_t* vertex = self->vertices + 0;
    glm::vec3 min_bounds = glm::vec3(vertex->position.x, vertex->position.y, vertex->position.z);
    glm::vec3 max_bounds = glm::vec3(vertex->position.x, vertex->position.y, vertex->position.z);

    for (int i = 0; i < self->vertex_count; i++) {
      vertex_t* vertex = self->vertices + i;

      min_bounds = glm::min(min_bounds, glm::vec3(vertex->position.x, vertex->position.y, vertex->position.z));
      max_bounds = glm::max(max_bounds, glm::vec3(vertex->position.x, vertex->position.y, vertex->position.z));
    }

    return max_bounds - min_bounds;
}

/**
 * Calculates the center shift of mesh vertices based on center of mass
 */
glm::vec3 mesh_calculate_center_shift(mesh_t* self) {
    glm::vec3 center_of_mass = glm::vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < self->vertex_count; i++) {
        vertex_t* vertex = self->vertices + i;
        center_of_mass += glm::vec3(vertex->position.x, vertex->position.y, vertex->position.z);
    }

    // Divide by the number of vertices to compute the center of mass
    if (self->vertex_count > 0) {
        center_of_mass /= static_cast<float>(self->vertex_count);
    }

    // Calculate the center shift as the difference between the center of mass
    // and the center of the bounding box (assuming that the bounding box is centered at the origin)
    glm::vec3 center_shift = center_of_mass - glm::vec3(0.0f, 0.0f, 0.0f);

    return center_shift;
}

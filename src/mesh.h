#pragma once

#include <string.h>

#include <glm/glm.hpp>

#include "camera.h"
#include "renderable.h"

typedef struct {
  char* name;
  vertex_t* vertices;
  GLsizei vertex_count;

  GLuint* indices;
  GLsizei index_count;

  renderable_t* renderable;
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

  mesh->renderable = renderable_create(
    vertices,
    vertex_count,
    indices,
    index_count,
    albedo_texture
  );

  return mesh;
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
glm::vec3 mesh_calculate_center_shift(mesh_t* self, glm::vec3 scale) {
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

    return center_shift; // * scale;
}

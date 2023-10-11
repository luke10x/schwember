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


// *********************************************************************
// This mesh is just an adapter
// ---
// TODO there is a method for renderable like that
// But this method should prevail,
// renderable_shape_rectangular_prism_create should be removed
// because vertex array is not packed in renderable
// but it is packed in mesh and it is useful
// *********************************************************************

mesh_t* mesh_shape_rectangular_prism_create(glm::vec3 dimensions) {

    // Define the vertices of the rectangular prism.
    vertex_t vertices[] = {
      // Front face
      { .position = { -dimensions.x, -dimensions.y, dimensions.z } },
      { .position = {  dimensions.x, -dimensions.y, dimensions.z } },
      { .position = {  dimensions.x,  dimensions.y, dimensions.z } },
      { .position = { -dimensions.x,  dimensions.y, dimensions.z } },

      // Back face
      { .position = {  dimensions.x, -dimensions.y, -dimensions.z } },
      { .position = { -dimensions.x, -dimensions.y, -dimensions.z } },
      { .position = { -dimensions.x,  dimensions.y, -dimensions.z } },
      { .position = {  dimensions.x,  dimensions.y, -dimensions.z } },

      // Top face
      { .position = {-dimensions.x, dimensions.y,  dimensions.z } },
      { .position = { dimensions.x, dimensions.y,  dimensions.z } },
      { .position = { dimensions.x, dimensions.y, -dimensions.z } },
      { .position = {-dimensions.x, dimensions.y, -dimensions.z } },

      // Bottom face
      { .position = {-dimensions.x, -dimensions.y, -dimensions.z } },
      { .position = { dimensions.x, -dimensions.y, -dimensions.z } },
      { .position = { dimensions.x, -dimensions.y,  dimensions.z } },
      { .position = {-dimensions.x, -dimensions.y,  dimensions.z } },

      // Left face
      { .position = { -dimensions.x, -dimensions.y, -dimensions.z } },
      { .position = { -dimensions.x, -dimensions.y,  dimensions.z } },
      { .position = { -dimensions.x,  dimensions.y,  dimensions.z } },
      { .position = { -dimensions.x,  dimensions.y, -dimensions.z } },

      // Right face
      { .position = { dimensions.x, -dimensions.y,  dimensions.z } },
      { .position = { dimensions.x, -dimensions.y, -dimensions.z } },
      { .position = { dimensions.x,  dimensions.y, -dimensions.z } },
      { .position = { dimensions.x,  dimensions.y,  dimensions.z } }
    };

    GLuint indices[] = {
        // Front face (two triangles)
        0, 1, 2,
        2, 3, 0,

        // Back face (two triangles)
        4, 5, 6,
        6, 7, 4,

        // Top face (two triangles)
        8, 9, 10,
        10, 11, 8,

        // Bottom face (two triangles)
        12, 13, 14,
        14, 15, 12,

        // Left face (two triangles)
        16, 17, 18,
        18, 19, 16,

        // Right face (two triangles)
        20, 21, 22,
        22, 23, 20
    };

    return mesh_create(
      "name of mesh",
      (vertex_t*) &vertices, (GLsizei) 24,
      (GLuint*) &indices, (GLsizei) 36,
      (texture_t*) NULL
    );
}

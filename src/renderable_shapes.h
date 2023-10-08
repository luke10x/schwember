#pragma once

#include <glm/glm.hpp>

#include "renderable.h"
#include <glm/glm.hpp>

renderable_t* renderable_shape_rectangular_prism_create(glm::vec3 dimensions) {

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

    return renderable_create(
      (vertex_t*) &vertices, (GLsizei) 24,
      (GLuint*) &indices, (GLsizei) 36,
      (texture_t*) NULL
    );


}

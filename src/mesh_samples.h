#pragma once

#include "mesh.h"
#include "texture.h"

/* *********************************************************************
 * Creates lamp mesh
 * ********************************************************************/
mesh_t* mesh_sample_create_lamp()
{
    // clang-format off
    vertex_t lightVertices[] =
    {
        { .position = {-0.1f, -0.1f,  0.1f} },
        { .position = {-0.1f, -0.1f, -0.1f} },
        { .position = {0.1f, -0.1f,  -0.1f} },
        { .position = {0.1f, -0.1f,   0.1f} },
        { .position = {-0.1f,  0.1f,  0.1f} },
        { .position = {-0.1f,  0.1f, -0.1f} },
        { .position = {0.1f,  0.1f,  -0.1f} },
        { .position = {0.1f,  0.1f,   0.1f} }
    };

    GLuint lightIndices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };
    // clang-format on

    return mesh_create(
        "light", lightVertices,
        sizeof(lightVertices) / sizeof(vertex_t), lightIndices,
        sizeof(lightIndices) / sizeof(GLuint), NULL
    );
}

/* *********************************************************************
 * Creates pyramid
 * ********************************************************************/
mesh_t* mesh_sample_create_pyramid()
{
    // clang-format off
    vertex_t vertices[] =
    {
        // { .position = {-0.5f, 0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
        // { .position = {-0.5f, 0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 5.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
        // { .position = {0.5f,  0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 5.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
        // { .position = {0.5f,  0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
        // { .position = {-0.5f, 0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = { -0.8f, 0.5f,  0.0f }}, // Left Side
        // { .position = {-0.5f, 0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = { -0.8f, 0.5f,  0.0f }}, // Left Side
        // { .position = {0.0f,  0.8f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 0.0, .normal = { -0.8f, 0.5f,  0.0f }}, // Left Side
        // { .position = {-0.5f, 0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, 0.5f, -0.8f }}, // Non-facing side
        // { .position = {0.5f,  0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, 0.5f, -0.8f }}, // Non-facing side
        // { .position = {0.0f,  0.8f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 0.0, .normal = {  0.0f, 0.5f, -0.8f }}, // Non-facing side
        // { .position = {0.5f,  0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.8f, 0.5f,  0.0f }}, // Right side
        // { .position = {0.5f,  0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.8f, 0.5f,  0.0f }}, // Right side
        // { .position = {0.0f,  0.8f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 0.0, .normal = {  0.8f, 0.5f,  0.0f }}, // Right side
        // { .position = {0.5f,  0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 1.0, .normal = {  0.0f, 0.5f,  0.8f }}, // Facing side
        // { .position = {-0.5f, 0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 1.0, .normal = {  0.0f, 0.5f,  0.8f }}, // Facing side
        // { .position = {0.0f,  0.8f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 1.0, .normal = {  0.0f, 0.5f,  0.8f }}  // Facing side

        { .position = {-0.5f, -0.4f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
        { .position = {-0.5f, -0.4f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 5.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
        { .position = {0.5f,  -0.4f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 5.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
        { .position = {0.5f,  -0.4f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
        { .position = {-0.5f, -0.4f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = { -0.8f, 0.5f,  0.0f }}, // Left Side
        { .position = {-0.5f, -0.4f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = { -0.8f, 0.5f,  0.0f }}, // Left Side
        { .position = {0.0f,  +0.4f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 0.0, .normal = { -0.8f, 0.5f,  0.0f }}, // Left Side
        { .position = {-0.5f, -0.4f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, 0.5f, -0.8f }}, // Non-facing side
        { .position = {0.5f,  -0.4f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, 0.5f, -0.8f }}, // Non-facing side
        { .position = {0.0f,  +0.4f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 0.0, .normal = {  0.0f, 0.5f, -0.8f }}, // Non-facing side
        { .position = {0.5f,  -0.4f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.8f, 0.5f,  0.0f }}, // Right side
        { .position = {0.5f,  -0.4f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.8f, 0.5f,  0.0f }}, // Right side
        { .position = {0.0f,  +0.4f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 0.0, .normal = {  0.8f, 0.5f,  0.0f }}, // Right side
        { .position = {0.5f,  -0.4f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 1.0, .normal = {  0.0f, 0.5f,  0.8f }}, // Facing side
        { .position = {-0.5f, -0.4f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 1.0, .normal = {  0.0f, 0.5f,  0.8f }}, // Facing side
        { .position = {0.0f,  +0.4f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 1.0, .normal = {  0.0f, 0.5f,  0.8f }}  // Facing side
    };
    
    GLuint indices[] =
    {
        0, 1, 2,    // Bottom side
        0, 2, 3,    // Bottom side
        4, 6, 5,    // Left side
        7, 9, 8,    // Non-facing side
        10, 12, 11, // Right side
        13, 15, 14  // Facing side
    };
    // clang-format on

    texture_t* texture_wall1 = texture_load_from_file(
        "img/01-16-stone.png", GL_TEXTURE0, GL_RGBA,
        GL_UNSIGNED_BYTE
    );

    return mesh_create(
        "pyramid_1", vertices, sizeof(vertices) / sizeof(vertex_t),
        indices, sizeof(indices) / sizeof(GLuint), texture_wall1
    );
}

/* *********************************************************************
 * Creates floor with checkers texture
 * ********************************************************************/
mesh_t* mesh_sample_create_floor()
{
    // clang-format off
    vertex_t vertices[] =
    {
        { .position = {-4.0f, 0.0f,  4.0f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = {  0.0f,  0.0f }, .tex_id = 0.0, .normal = {  0.0f, 1.0f, 0.0f }}, 
        { .position = {-4.0f, 0.0f, -4.0f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = {  0.0f, 01.0f }, .tex_id = 0.0, .normal = {  0.0f, 1.0f, 0.0f }}, 
        { .position = { 4.0f, 0.0f, -4.0f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 01.0f, 01.0f }, .tex_id = 0.0, .normal = {  0.0f, 1.0f, 0.0f }}, 
        { .position = { 4.0f, 0.0f,  4.0f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 01.0f,  0.0f }, .tex_id = 0.0, .normal = {  0.0f, 1.0f, 0.0f }}
    };

    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
    };
    // clang-format on

    texture_t* texture_floor = texture_load_from_file(
        "img/19-13-floor.png", GL_TEXTURE0, GL_RGBA,
        GL_UNSIGNED_BYTE
    );

    return mesh_create(
        "floor",                                        // Name
        vertices, sizeof(vertices) / sizeof(vertex_t),  // Vertices
        indices, sizeof(indices) / sizeof(GLuint),      // Indexes
        texture_floor                                   // Texture
    );
}

/* *********************************************************************
 * Creates ellipsis mesh
 * ********************************************************************/
mesh_t* mesh_sample_create_ellipsis(float radius, float height)
{
    int numTheta = 20;  // Number of divisions around the ellipsoid
    int numPhi   = 80;  // Number of divisions from pole to pole

    std::vector<vertex_t> vertices;
    std::vector<GLuint> indices;

    for (int i = 0; i < numTheta; i++) {
        for (int j = 0; j < numPhi; j++) {
            float theta = 2.0f * M_PI * i / (numTheta - 1);
            float phi   = M_PI * j / (numPhi - 1);

            float x = radius * cos(theta) * sin(phi);
            float y = height * cos(phi);
            float z = radius * sin(theta) * sin(phi);

            vertex_t vertex = {
                .position = {x, y, z},
                .color    = {0.83f, 0.10f, 0.14f},
                .texUV    = {0.0f, 0.0f},
                .tex_id   = 0.0,
                .normal   = {0.0f, 1.0f, 0.0f}
            };

            vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < numTheta - 1; i++) {
        for (int j = 0; j < numPhi - 1; j++) {
            int vertexIndex = i * numPhi + j;
            // First triangle
            indices.push_back(vertexIndex);
            indices.push_back(vertexIndex + 1);
            indices.push_back(vertexIndex + numPhi);

            // Second triangle
            indices.push_back(vertexIndex + 1);
            indices.push_back(vertexIndex + numPhi + 1);
            indices.push_back(vertexIndex + numPhi);
        }
    }

    mesh_t* result = mesh_create(
        "ellipsoid",                       // Name
        vertices.data(), vertices.size(),  // vertices
        indices.data(), indices.size(),    // indices
        NULL                               // textures
    );

    // delete vertices;
    // free(indices);

    return result;
}

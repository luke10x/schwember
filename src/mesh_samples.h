#pragma once

#include "texture.h"
#include "mesh.h"  
  
mesh_t* mesh_sample_create_lamp() {
  vertex_t lightVertices[] =
  { //     COORDINATES     //
    { .position = {-0.1f, -0.1f,  0.1f}},
    { .position = {-0.1f, -0.1f, -0.1f}},
    { .position = {0.1f, -0.1f,  -0.1f}},
    { .position = {0.1f, -0.1f,   0.1f}},
    { .position = {-0.1f,  0.1f,  0.1f}},
    { .position = {-0.1f,  0.1f, -0.1f}},
    { .position = {0.1f,  0.1f,  -0.1f}},
    { .position = {0.1f,  0.1f,   0.1f}}
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

  return mesh_create(
    "light",
    lightVertices, sizeof(lightVertices) / sizeof(vertex_t),
    lightIndices,  sizeof(lightIndices) / sizeof(GLuint),
    NULL
  );
}
mesh_t* mesh_sample_create_pyramid() {
  vertex_t vertices[] =
  {
    { .position = {-0.5f, 0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
    { .position = {-0.5f, 0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 5.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
    { .position = {0.5f,  0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 5.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
    { .position = {0.5f,  0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, -1.0f, 0.0f }}, // Bottom side
    { .position = {-0.5f, 0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = { -0.8f, 0.5f,  0.0f }}, // Left Side
    { .position = {-0.5f, 0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = { -0.8f, 0.5f,  0.0f }}, // Left Side
    { .position = {0.0f,  0.8f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 0.0, .normal = { -0.8f, 0.5f,  0.0f }}, // Left Side
    { .position = {-0.5f, 0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, 0.5f, -0.8f }}, // Non-facing side
    { .position = {0.5f,  0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.0f, 0.5f, -0.8f }}, // Non-facing side
    { .position = {0.0f,  0.8f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 0.0, .normal = {  0.0f, 0.5f, -0.8f }}, // Non-facing side
    { .position = {0.5f,  0.0f, -0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.8f, 0.5f,  0.0f }}, // Right side
    { .position = {0.5f,  0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 0.0, .normal = {  0.8f, 0.5f,  0.0f }}, // Right side
    { .position = {0.0f,  0.8f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 0.0, .normal = {  0.8f, 0.5f,  0.0f }}, // Right side
    { .position = {0.5f,  0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 5.0f, 0.0f }, .tex_id = 1.0, .normal = {  0.0f, 0.5f,  0.8f }}, // Facing side
    { .position = {-0.5f, 0.0f,  0.5f}, .color = { 0.83f, 0.70f, 0.44f }, .texUV = { 0.0f, 0.0f }, .tex_id = 1.0, .normal = {  0.0f, 0.5f,  0.8f }}, // Facing side
    { .position = {0.0f,  0.8f,  0.0f}, .color = { 0.92f, 0.86f, 0.76f }, .texUV = { 2.5f, 5.0f }, .tex_id = 1.0, .normal = {  0.0f, 0.5f,  0.8f }}  // Facing side
  };
  
  GLuint indices[] =
  {
    0, 1, 11, // Bottom side
    0, 2, 3, // Bottom side
    4, 6, 5, // Left side
    7, 9, 8, // Non-facing side
    10, 12, 11, // Right side
    13, 15, 14 // Facing side
  };

  texture_t* texture_wall1 = texture_create(
    "assets/png/01-16-stone.png",
     GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE
  );

  return mesh_create(
    "pyramid_1",
    vertices, sizeof(vertices) / sizeof(vertex_t),
    indices,  sizeof(indices) / sizeof(GLuint),
    texture_wall1
  );
}

mesh_t* mesh_sample_create_floor() {

  // Vertices coordinates
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
  
  texture_t* texture_floor = texture_create(
    "assets/png/19-13-floor.png",
     GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE
  );

  return mesh_create(
    "floor",
    vertices, sizeof(vertices) / sizeof(vertex_t),
    indices,  sizeof(indices) / sizeof(GLuint),
    texture_floor
  );
}
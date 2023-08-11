#pragma once

#include "gl.h"

#include <cglm/cglm.h>

// Structure to standardize the vertices used in the meshes
typedef struct
{
	vec3  position;
	vec3  color;
	vec2  texUV;
	float tex_id;
	vec3  normal;
} vertex_t;

/*
 * An Element Buffer Object (EBO) is a buffer
 * that stores indices for indexed rendering.
 * Instead of duplicating vertex data for each triangle,
 * a list of indices is used to specify which vertices to use.
 * This can save memory and improve performance in certain cases.
 */
typedef struct {
  GLuint ID;
} ebo_t;

ebo_t* ebo_create(GLuint* indices, GLsizeiptr size);

void ebo_bind  (ebo_t* self);

void ebo_unbind(ebo_t* self);

void ebo_delete(ebo_t* self);

/*
 * A Vertex Buffer Object (VBO) is a buffer
 * that stores vertex data such as vertex positions,
 * normals, texture coordinates, and other per-vertex data.
 * VBOs are used to transfer large amounts of vertex data
 * to the GPU efficiently.
 */
typedef struct {
  GLuint ID;
  GLsizei vertice_count;
} vbo_t;

vbo_t* vbo_create(vertex_t* vertices, GLsizei size);

void vbo_bind (vbo_t* self);

void vbo_unbind(vbo_t* self);

void vbo_delete(vbo_t* self);

/*
 * A Vertex Array Object (VAO) is an object
 * that encapsulates all the state needed to specify
 * how to access vertex data in a buffer object.
 * It specifies the format of the vertex data
 * and how to read it from the Vertex Buffer Objects (VBOs).
 */
typedef struct {
  GLuint ID;
} vao_t;

vao_t* vao_create();

void vao_link_attrib(
  vao_t* self,
  vbo_t* vbo,
  GLuint layout,
  GLuint numComponents,
  GLenum type,
  GLsizeiptr stride,
  void* offset);

void vao_bind  (vao_t* self);

void vao_unbind(vao_t* self);

// Implementation

#include <stdio.h>
#include <stdlib.h>

#include "buffers.h"

ebo_t* ebo_create(GLuint* indices, GLsizeiptr size) {
  ebo_t* self = (ebo_t*) malloc(sizeof(ebo_t));
  glGenBuffers(1, &(self->ID));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);  
  return self;
}

void ebo_bind  (ebo_t* self) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->ID);
}

void ebo_unbind(ebo_t* self) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ebo_delete(ebo_t* self) {
  // glDeleteBuffers(1, self->ID);
  free(self);
}

vbo_t* vbo_create(vertex_t* vertices, GLsizei size) {
  vbo_t* self = (vbo_t*) malloc(sizeof(vbo_t));
  glGenBuffers(1, &(self->ID));
  glBindBuffer(GL_ARRAY_BUFFER, self->ID);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  self->vertice_count = size; 
  return self;
}

void vbo_bind  (vbo_t* self) {
  glBindBuffer(GL_ARRAY_BUFFER, self->ID);
}

void vbo_unbind(vbo_t* self) {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_delete(vbo_t* self) {
  // glDeleteBuffers(1, self->ID);
  free(self);
}

vao_t* vao_create() {
  vao_t* self = (vao_t*) malloc(sizeof(vao_t));
  glGenVertexArrays(1, &(self->ID));
  return self;
}

void vao_link_attrib(vao_t* self, vbo_t* vbo, GLuint attribIndex, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
  vbo_bind(vbo);

  // Sets up a vertex attribute pointer
  glVertexAttribPointer(attribIndex, numComponents, type, GL_FALSE, stride, offset);
  glEnableVertexAttribArray(attribIndex);

  vbo_unbind(vbo);
}

void vao_bind  (vao_t* self) {
  glBindVertexArray(self->ID);
}

void vao_unbind(vao_t* self) {
  glBindVertexArray(0);
}
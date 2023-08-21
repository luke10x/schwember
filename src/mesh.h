#pragma once

#include <string.h>

#include "buffers.h"
#include "camera.h"
#include "texture.h"

#pragma once

typedef struct {
  char* name;
  vertex_t* vertices;
  GLsizei vertex_count;

  GLuint* indices;
  GLsizei index_count;

  texture_t* textures;
  GLsizei texture_count;

  vao_t* vao;
} mesh_t;


mesh_t* mesh_create(
  const char* name,
  vertex_t* vertices,
  GLsizei vertex_count,
  GLuint* indices,
  GLsizei index_count,
  texture_t** textures,
  GLsizei texture_count
);

void mesh_draw(mesh_t* mesh, shader_t* shader, camera_t* camera);

// Implementation
#include "mesh.h"

mesh_t* mesh_create(
  const char* name,
  vertex_t* vertices,
  GLsizei vertex_count,
  GLuint* indices,
  GLsizei index_count,
  texture_t** textures,
  GLsizei texture_count
) {
  if (textures == NULL) {
    printf("texture not set while trying to create mesh for \"%s\"\n", name);
  }

  mesh_t* mesh = (mesh_t*) malloc(sizeof(mesh_t));

  // TODO copy vertices and indices, becuase it is owned by outside code,
  // and can be gone at any moment.
  // Yet, these are not used anyway but in case they are planned to be used
  // then thy must be copied
  mesh->vertices = vertices;
  mesh->vertex_count = vertex_count;

  mesh->indices = indices;
  mesh->index_count = index_count;

  // Probably it makes more sense that it is allready packed to textures blob
  // Texture Array Pointer (texture_t**) should not be used,
  // Texture Object Array (texture*) should be used instead.
  // texture* can be looped using indexing syntax with the address-of operator
  mesh->textures = (texture_t*)malloc(4 * sizeof(texture_t));
  for (int i = 0; i < texture_count; i++) {
    texture_t* addr = &(mesh->textures[i]);
    memcpy(addr,  textures[i], sizeof(texture_t));
  }
  mesh->texture_count = texture_count;

  mesh->vao = vao_create();

  vao_bind(mesh->vao);

  vbo_t* vbo = vbo_create(mesh->vertices, mesh->vertex_count * sizeof(vertex_t));
  ebo_t* ebo = ebo_create(mesh->indices,  mesh->index_count  * sizeof(GLuint));

	// Links VBO attributes such as coordinates and colors to VAO
	vao_link_attrib(mesh->vao, vbo, 0, 3, GL_FLOAT, sizeof(vertex_t), (void*)0);                   // position
	vao_link_attrib(mesh->vao, vbo, 1, 3, GL_FLOAT, sizeof(vertex_t), (void*)(3 * sizeof(float))); // color
	vao_link_attrib(mesh->vao, vbo, 2, 2, GL_FLOAT, sizeof(vertex_t), (void*)(6 * sizeof(float))); // tex_UV
	vao_link_attrib(mesh->vao, vbo, 3, 1, GL_FLOAT, sizeof(vertex_t), (void*)(8 * sizeof(float))); // tex_id
	vao_link_attrib(mesh->vao, vbo, 4, 3, GL_FLOAT, sizeof(vertex_t), (void*)(9 * sizeof(float))); // normal
	
  // Unbind all to prevent accidentally modifying them
  vao_unbind(mesh->vao);
  vbo_unbind(vbo);
  ebo_unbind(ebo);

  return mesh;
}

void mesh_draw(mesh_t* mesh, shader_t* shader, camera_t* camera) {

  shader_activate(shader);

  if (mesh->texture_count > 0) {
    texture_unit(&(mesh->textures[0]), shader, "sampler[0]", 0);
    texture_bind(&(mesh->textures[0]));
  }
  if (mesh->texture_count > 1) {
    texture_unit(&(mesh->textures[1]), shader, "sampler[1]", 1);
    texture_bind(&(mesh->textures[1]));
  }
  if (mesh->texture_count > 2) {
    texture_unit(&(mesh->textures[2]), shader, "sampler[2]", 2);
    texture_bind(&(mesh->textures[2]));
  }
  if (mesh->texture_count > 3) {
    texture_unit(&(mesh->textures[3]), shader, "sampler[3]", 3);
    texture_bind(&(mesh->textures[3]));
  }

	// Take care of the camera Matrix
	glUniform3f(
    glGetUniformLocation(shader->ID, "camPos"),
    camera->view_matrix[3][0],
    camera->view_matrix[3][1],
    camera->view_matrix[3][2]
  );

  camera_matrix(camera, shader, "worldToView");
  
  vao_bind(mesh->vao);

	glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
  
}

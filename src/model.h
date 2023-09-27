#pragma once

#include "mesh.h"

typedef struct {
  
} material_t;

typedef struct {
  mesh_t** meshes;
  size_t num_meshes;

  material_t** materials;
  size_t num_materials;

  int* primitive_material_map;
} model_t;

model_t* model_create() {
  model_t* self = (model_t*) malloc(sizeof(model_t));

  self->meshes = (mesh_t**) malloc(128 * sizeof(mesh_t*));
  self->num_meshes = 0;

  self->num_materials = 0;
  return self;
}

void model_draw(model_t* self, shader_t* shader, camera_t* camera) {
  for (int i = 0; i < self->num_meshes; i++) {
    mesh_t* each_mesh = self->meshes[i];
    mesh_draw(each_mesh, shader, camera);
  }
}

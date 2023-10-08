#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "renderable.h"
#include "renderable_shapes.h"
#include "collider.h"
#include "model.h"
#include "model_load.h"

typedef struct {
  glm::mat4     transform;
  shader_t*     shader;
  camera_t*     camera;
  renderable_t* visual;
  collider_t*   bumper;
  renderable_t* bumper_visual;
  glm::mat4     bumper_transform;
  bool          debug;
} pc_t;

pc_t* pc_create(shader_t* visual_shader) {
  pc_t* self = (pc_t*) malloc(sizeof(pc_t));

  self->shader = visual_shader;

  self->transform = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 4.0f));
 
 self->transform = glm::scale(self->transform, glm::vec3(0.005f, 0.005f, 0.005f));

  model_t* visual_model = model_create();
  model_load_from_file(visual_model, "assets/gltf/stickman_low_poly.glb");
  mesh_t* visual_mesh = ((model_t*) visual_model)->meshes[0];

  self->bumper_visual = renderable_shape_rectangular_prism_create(glm::vec3(0.5f, 0.55f, 0.25f));
    // Extract only the translation and rotation components into a new matrix
  self->bumper_transform = glm::mat4_cast(glm::quat_cast(self->transform));
  self->bumper_transform = glm::translate(self->bumper_transform, glm::vec3(0.0f, 0.25f, 0.0f));

    // newMatrix[3] = originalMatrix[3]; 


  self->visual = renderable_create(
    visual_mesh->vertices,
    visual_mesh->vertex_count,
    visual_mesh->indices,
    visual_mesh->index_count,
    NULL
  );

  return self;
}

void pc_draw(pc_t* self, camera_t* camera) {

  shader_activate(self->shader);
  shader_set_uniform_mat4(self->shader, "modelToWorld", self->transform);
  renderable_draw(self->visual, self->shader, camera);

  // Copy translation and rotation components into a bumper transform
  self->bumper_transform  = glm::mat4_cast(glm::quat_cast(self->transform));
  self->bumper_transform[3] = self->transform[3]; 
  
  glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 4.0f));
  shader_set_uniform_mat4(self->shader, "modelToWorld", self->bumper_transform);

  renderable_draw(self->bumper_visual, self->shader, camera);



}

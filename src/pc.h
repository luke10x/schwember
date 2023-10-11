#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "renderable.h"
#include "renderable_shapes.h"
#include "collider.h"
#include "model.h"
#include "model_load.h"
#include "physics.h"

typedef struct {
  glm::mat4     transform;
  shader_t*     shader;
  camera_t*     camera;
  renderable_t* visual;
  collider_t*   collider;
  renderable_t* bumper_visual;
  glm::mat4     bumper_transform;
  collider_t*   bumper_collider;
  bool          debug;
  physics_t*    physics;
} pc_t;

pc_t* pc_create(shader_t* visual_shader, physics_t* physics) {
  pc_t* self = (pc_t*) malloc(sizeof(pc_t));

  self->shader = visual_shader;

  self->physics = physics;

  self->transform = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 15.0f, 5.0f));
 
  self->transform = glm::scale(self->transform, glm::vec3(0.005f, 0.005f, 0.005f));

  model_t* visual_model = model_create();
  model_load_from_file(visual_model, "assets/gltf/stickman_low_poly.glb");
  mesh_t* visual_mesh = (mesh_t*) visual_model->meshes[0];
  self->visual = renderable_create(
    visual_mesh->vertices,
    visual_mesh->vertex_count,
    visual_mesh->indices,
    visual_mesh->index_count,
    NULL
  );

  self->collider = collider_create_box_from_mesh(
    self->transform,
    visual_mesh,
    self->physics
  );
  mesh_t* bumper_mesh = mesh_shape_rectangular_prism_create(glm::vec3(0.5f, 0.55f, 0.25f));
  self->bumper_visual = renderable_create(
    bumper_mesh->vertices, bumper_mesh->vertex_count,
    bumper_mesh->indices, bumper_mesh->index_count,
    NULL
  );

  // Extract only the translation and rotation components into a new matrix
  self->bumper_transform = glm::mat4_cast(glm::quat_cast(self->transform));
  self->bumper_transform = glm::translate(self->bumper_transform, glm::vec3(0.0f, 0.25f, 0.0f));
  // newMatrix[3] = originalMatrix[3]; 

  self->bumper_collider = collider_create_box_from_mesh(
    self->bumper_transform,
    bumper_mesh,
    self->physics
  );
  
  return self;
}

void pc_draw(pc_t* self, camera_t* camera) {
  
    // This will be set be reset anyway for now
    self->bumper_transform = collider_update_transform(
        self->bumper_collider,
        self->bumper_transform
    );
    self->transform = collider_update_transform(
        self->collider,
        self->transform
    );

  shader_activate(self->shader);
  shader_set_uniform_mat4(self->shader, "modelToWorld", self->transform);
  renderable_draw(self->visual, self->shader, camera);

  // Copy translation and rotation components into a bumper transform
  self->bumper_transform  = glm::mat4_cast(glm::quat_cast(self->transform));
  self->bumper_transform[3] = self->transform[3]; 
  
  shader_set_uniform_mat4(self->shader, "modelToWorld", self->bumper_transform);

  renderable_draw(self->bumper_visual, self->shader, camera);
}


void pc_handle_event(pc_t *self, uint8_t event) {
    if (event == CONTROL_PC_FORWARD) {

        btScalar friction = 0.0f;
        btScalar rollingFriction = 0.0f;
        btScalar restitution = 0.6f;
        ((collider_box_t*) self->collider)->rigid_body->setFriction(friction);
        ((collider_box_t*) self->collider)->rigid_body->setRestitution(restitution);
        ((collider_box_t*) self->collider)->rigid_body->setRollingFriction(rollingFriction);

        btVector3 forceToApply(0.5f, 0.0f, 0.0f); // A small force to lift the body
        btRigidBody* fallenBody = ((collider_box_t*) self->collider)->rigid_body;
        fallenBody->setActivationState(1);

        fallenBody->applyForce(forceToApply, fallenBody->getCenterOfMassPosition());
    }
} 

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "collider.h"
#include "model.h"
#include "model_load.h"
#include "physics.h"
#include "renderable.h"
#include "renderable_shapes.h"

/* *********************************************************************
 * PC
 * ********************************************************************/
typedef struct pc_t {
    glm::mat4 transform;
    shader_t *shader;
    camera_t *camera;
    renderable_t *renderable;
    float model_scale;

    collider_t *collider;
    glm::mat4 collider_transform;
    mesh_t *collider_mesh;  // TODO collider only needs renderable
                            // Not mesh
    bool debug;
    shader_t *debug_shader;
    physics_t *physics;

    glm::vec3 model_offset;
} pc_t;

/* *********************************************************************
 * PC constructor
 * ********************************************************************/
pc_t *pc_create(
    shader_t *renderable_shader,
    shader_t *debug_shader,
    physics_t *physics
)
{
    pc_t *self = (pc_t *) malloc(sizeof(pc_t));

    self->shader       = renderable_shader;
    self->debug_shader = debug_shader;

    self->physics = physics;

    self->model_scale = 0.0045;

    glm::vec3 position = glm::vec3(-6.0f, 4.0f, 8.0f);
    self->transform    = glm::translate(glm::mat4(1.0f), position);
    self->collider_transform =
        glm::translate(glm::mat4(1.0f), position);

    // Create renderable
    model_t *renderable_model = model_create();
    model_load_from_file(
        renderable_model, "assets/gltf/stickman_low_poly.glb"
    );
    mesh_t *renderable_mesh = (mesh_t *) renderable_model->meshes[0];
    self->renderable        = renderable_create(
        renderable_mesh->vertices, renderable_mesh->vertex_count,
        renderable_mesh->indices, renderable_mesh->index_count, NULL
    );
    glm::vec3 box = mesh_calculate_bounding_box(renderable_mesh) *
                    (self->model_scale * 0.5f);
    
    // Because center of the mesh is not in 0,0,0 point
    // TODO calculate this correction from vertices
    glm::vec3 center_correction = glm::vec3(0.0f, box.y * 0.5f, 0.0f);

    // Store half of height, to allign center with the feet
    self->model_offset = glm::vec3(0.0f, box.y * 0.5f, 0.0f) + center_correction;

    float reach    = 2.0f;
    self->collider = collider_create_capsule(
        self->collider_transform,  // transform
        box.z * reach,             // radius
        box.y,                     // height
        self->physics              // physics
    );
    btRigidBody *rigid_body =
        ((collider_sphere_t *) self->collider)->rigid_body;

    // Make it bouncy
    rigid_body->setRestitution(2.0f);

    // Make it not spin when hits a wall
    rigid_body->setRollingFriction(1.0f);

    // Do not stop reacting to forces when reaches ground
    rigid_body->setActivationState(DISABLE_DEACTIVATION);

    // Make it always stand
    rigid_body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

    rigid_body->setDamping(0.75f, 0.99f);

    self->collider_mesh =
        mesh_sample_create_ellipsis(box.z * reach, box.y);

    return self;
}

/* *********************************************************************
 * Draw
 * ********************************************************************/
void pc_draw(pc_t *self, camera_t *camera)
{
    // Collider transform is updated byt the laws of physics
    self->collider_transform = collider_update_transform(
        self->collider, self->collider_transform
    );

    // Transform is then updated by collider transform
    // self->transform =
    //     glm::translate(self->transform, glm::vec3(0.0f, 0.25f, 0.0f));
    // Take back the offset
    self->transform = glm::translate(
        self->transform, -self->model_offset
    );
    // Apply scaled factor on transform, because model is way too big
    self->transform =
        glm::scale(self->transform, glm::vec3(self->model_scale));



    // Draw the figurine
    shader_activate(self->shader);
    shader_set_uniform_mat4(
        self->shader, "modelToWorld", self->transform
    );
    renderable_draw(self->renderable, self->shader, camera);

    // Draw collider mesh renderable for debug
    shader_activate(self->debug_shader);
    shader_set_uniform_mat4(
        self->debug_shader, "modelToWorld", self->collider_transform
    );
    renderable_draw(
        self->collider_mesh->renderable, self->debug_shader, camera
    );
}

/* *********************************************************************
 * Handle event
 *
 * PC should be controlled through this method only
 * ********************************************************************/
void pc_handle_event(pc_t *self, uint8_t event)
{
    btVector3 velocity;

    if (event == CONTROL_PC_FORWARD) {
        velocity = btVector3(0.0f, 0.0f, 1.0f);
    } else if (event == CONTROL_PC_BACK) {
        velocity = btVector3(0.0f, 0.0f, -1.0f);
    } else if (event == CONTROL_PC_LEFT) {
        velocity = btVector3(1.0f, 0.0f, 0.0f);
    } else if (event == CONTROL_PC_RIGHT) {
        velocity = btVector3(-1.0f, 0.0f, 0.0f);
    } else {
        // event is not actionable by PC
        return;
    }

    // If reached until this point means some movement has to happen
    ((collider_box_t *) self->collider)
        ->rigid_body->setLinearVelocity(velocity * 3.0f);
}

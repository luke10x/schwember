#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "physics.h"

#define COLLIDER_TYPE_PLANE 1
#define COLLIDER_TYPE_BOX   2
#define COLLIDER_TYPE_MESH  3

typedef struct {
  uint8_t type;
} collider_t;

typedef struct {
  collider_t        parent;
  btRigidBody*      rigid_body;
  btCollisionShape* collision_shape; 
} collider_plane_t;

///////////////////////////////////////////////////////////////////////////////
// Create plane collider                                                     //
///////////////////////////////////////////////////////////////////////////////
collider_t* collider_create_plane(
  glm::mat4  initial_transform,
  physics_t* physics
) {
  collider_plane_t* self = (collider_plane_t*) malloc(
    sizeof(collider_plane_t)
  );

  self->parent.type = COLLIDER_TYPE_PLANE;
  
  // Plane params
  btVector3 planeNormal(0, 1, 0);
  btScalar planeConstant = 0.0;

  // Create a btStaticPlaneShape
  self->collision_shape = new btStaticPlaneShape(
    planeNormal,
    planeConstant
  );

  // Add collision shape to physics engine
  physics->collision_shapes
    .push_back(self->collision_shape);

  printf("added to col shapees \n");
  // Bullet uses its own format to hold transform
  btTransform bt_transform;
  bt_transform.setFromOpenGLMatrix(
    glm::value_ptr(initial_transform)
  );

  // Create motion state
  btScalar mass(0.0);
  btVector3 local_inertia(0, 0, 0);
	btDefaultMotionState* motion_state = new btDefaultMotionState(
    bt_transform
  );

  // Create rigid body
  btRigidBody::btRigidBodyConstructionInfo floor_rb_info(
    mass, motion_state, self->collision_shape, local_inertia
  );
  self->rigid_body = new btRigidBody(floor_rb_info);

  // Add rigid body to physics engine
  physics->dynamics_world
    ->addRigidBody(self->rigid_body);

  return (collider_t*) self;
}

collider_t* collider_create_box_from_mesh(glm::mat4 initial_transform) {

}


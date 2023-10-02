#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "physics.h"

typedef struct {
  btRigidBody*      rigid_body;
  btCollisionShape* collision_shape; 
  glm::vec3         bounding_box; // NULL for plane
} body_t;

body_t* body_create_plane(
  glm::mat4  initial_transform
) {
  body_t* self = (body_t*) malloc(sizeof(body_t));  

  btVector3 planeNormal(0, 1, 0);
  btScalar planeConstant = 0.0;

  // Create a btStaticPlaneShape
  self->collision_shape = new btStaticPlaneShape(
    planeNormal,
    planeConstant
  );

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

  return self;
}
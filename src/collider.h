#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "physics.h"
#include "mesh.h"

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

typedef struct {
  collider_t        parent;
  btRigidBody*      rigid_body;
  btCollisionShape* collision_shape;
  glm::vec3         center_shift;
} collider_box_t;

////////////////////////////////////////////////////////////////////////
// Create plane collider                                              //
////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////
// Create box collider                                                //
////////////////////////////////////////////////////////////////////////
collider_t* collider_create_box_from_mesh(
  glm::mat4  initial_transform,
  mesh_t*    mesh,
  physics_t* physics
) {
  collider_box_t* self = (collider_box_t*) malloc(
    sizeof(collider_box_t)
  );
  self->parent.type = COLLIDER_TYPE_BOX;

  // Extract scale from initial_transform
  glm::vec3 scale;
  scale.x = glm::length(initial_transform[0]) ;
  scale.y = glm::length(initial_transform[1]) ;
  scale.z = glm::length(initial_transform[2]) ;

  // Calculate box parameters from mesh
  glm::vec3 bounding_box = mesh_calculate_bounding_box(mesh, scale);

  // Create collision shape from bounding box
  self->collision_shape = new btBoxShape(btVector3(
    bounding_box.x,
    bounding_box.y,
    bounding_box.z
  ));

  // Use this as a storage for scaling
  // probably misusing Bullet here, but this local scaling,
  // is used in collider_update_transform()
  self->collision_shape
    ->setLocalScaling(btVector3(scale.x, scale.y, scale.z));

  // Add collision shape to physics engine
  physics->collision_shapes
    .push_back(self->collision_shape);

  // Bullet uses its own format to hold transform
  btTransform bt_transform;
  bt_transform.setFromOpenGLMatrix(
    glm::value_ptr(initial_transform)
  );

  // Create motion state
  btScalar mass(100.0); // must be not 0 with dynamics
	btVector3 local_inertia(0, 0, 0);

  // When has mass add inertia
  self->collision_shape->calculateLocalInertia(mass, local_inertia);

  btDefaultMotionState* motion_state = new btDefaultMotionState(
    bt_transform
  );

  // Create rigid body
  btRigidBody::btRigidBodyConstructionInfo rb_info(
    mass, motion_state, self->collision_shape, local_inertia
  );
  self->rigid_body = new btRigidBody(rb_info);

  // Add rigid body to physics engine
  physics->dynamics_world
    ->addRigidBody(self->rigid_body);

  return (collider_t*) self;
}

////////////////////////////////////////////////////////////////////////
// update transform based on physics changes                          //
////////////////////////////////////////////////////////////////////////
glm::mat4  collider_update_transform(
  collider_t* self,
  glm::mat4 target_transform
) {
  if (self->type == COLLIDER_TYPE_BOX) {
    collider_box_t* impl = (collider_box_t*) self;
    
    btTransform trans;
    impl->rigid_body
      ->getMotionState()
      ->getWorldTransform(trans);

    // fills in changes to target_transform
    trans.getOpenGLMatrix(glm::value_ptr(target_transform));

    // Take local scaling back from collision shape.
    // And apply it on transform.
    // It does not seem that this local scaling is used by Bullet,
    // which feels strange. An I am probably misusing it.
    // local scaling is set in create_collider_X() functions
    // of this unit.
    // TODO: Learn if this is the right way of doing it
    btVector3 bt_scale = impl->collision_shape->getLocalScaling();
    target_transform[0] *= bt_scale.getX();
    target_transform[1] *= bt_scale.getY();
    target_transform[2] *= bt_scale.getZ();

    return target_transform;
  }
}

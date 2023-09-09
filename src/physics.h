#pragma once

#include <btBulletDynamicsCommon.h>


typedef struct {
  btDefaultCollisionConfiguration*        collision_configuration;

  // Collision dispatcher, handles collision  
  btCollisionDispatcher*                  dispatcher;
  
  // Broadphase interface, detects overlapping objects
  btBroadphaseInterface*                  overlapping_pair_cache;
  
  // Constraint solver
  btSequentialImpulseConstraintSolver*    solver;

  // The world where physics takes place
  btDiscreteDynamicsWorld*                dynamics_world;

  // Keeps track of collision shapes
  btAlignedObjectArray<btCollisionShape*> collision_shapes;

} physics_t;

/**
 * Constructor
 */
physics_t* physics_create() {
  physics_t* self = (physics_t*) malloc(sizeof(physics_t));

	// Initialize Bullet
	self->collision_configuration = new btDefaultCollisionConfiguration();
	self->dispatcher = new btCollisionDispatcher(
    self->collision_configuration
  );
	self->overlapping_pair_cache = new btDbvtBroadphase();
	self->solver = new btSequentialImpulseConstraintSolver;

	self->dynamics_world = new btDiscreteDynamicsWorld(
    self->dispatcher,
    self->overlapping_pair_cache,
    self->solver,
    self->collision_configuration
  );

	self->dynamics_world->setGravity(btVector3(0, -10, 0));

  return self;
}

/**
 * Destructor
 */
void physics_delete(physics_t* self) {

	// Remove rigid bodies
  const int num_collision_objects = self->dynamics_world->getNumCollisionObjects();
	for (int i = num_collision_objects - 1; i >= 0; i--) {
		btCollisionObject* obj = self->dynamics_world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		self->dynamics_world->removeCollisionObject(obj);
		delete obj;
	}

	// Delete collision shapes
	for (int i = 0; i < self->collision_shapes.size(); i++) {
		btCollisionShape* shape = self->collision_shapes[i];
		self->collision_shapes[i] = 0;
		delete shape;
	}

	// TODO Unload physics objects
	// for (auto& object : physics_objects) {
	// 	object.unload();
	// }

	delete self->dynamics_world;
	delete self->solver;
	delete self->overlapping_pair_cache;
	delete self->dispatcher;
	delete self->collision_configuration;

  delete self;
}
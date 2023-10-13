#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "mesh.h"
#include "physics.h"

#define COLLIDER_TYPE_PLANE  1
#define COLLIDER_TYPE_BOX    2
#define COLLIDER_TYPE_MESH   3
#define COLLIDER_TYPE_SPHERE 4

typedef struct {
    uint8_t type;
    physics_t* physics;
} collider_t;

typedef struct {
    collider_t parent;
    btRigidBody* rigid_body;
    btCollisionShape* collision_shape;
} collider_plane_t;

typedef struct {
    collider_t parent;
    btRigidBody* rigid_body;
    btCollisionShape* collision_shape;
    glm::vec3 center_shift;
} collider_box_t;

typedef struct {
    collider_t parent;
    btRigidBody* rigid_body;
    btCollisionShape* collision_shape;
} collider_sphere_t;

typedef struct {
    collider_t parent;
    btRigidBody* rigid_body;
    btCollisionShape* collision_shape;
} collider_mesh_t;

/* *********************************************************************
 * Plane collider construct
 * ********************************************************************/
collider_t*
collider_create_plane(glm::mat4 initial_transform, physics_t* physics)
{
    collider_plane_t* self =
        (collider_plane_t*) malloc(sizeof(collider_plane_t));

    self->parent.type    = COLLIDER_TYPE_PLANE;
    self->parent.physics = physics;

    // Plane params
    btVector3 planeNormal(0, 1, 0);
    btScalar planeConstant = 0.0;

    // Create a btStaticPlaneShape
    self->collision_shape =
        new btStaticPlaneShape(planeNormal, planeConstant);

    // Add collision shape to physics engine
    physics->collision_shapes.push_back(self->collision_shape);

    // Bullet uses its own format to hold transform
    btTransform bt_transform;
    bt_transform.setFromOpenGLMatrix(glm::value_ptr(initial_transform));

    // Create motion state
    btScalar mass(0.0);
    btVector3 local_inertia(0, 0, 0);
    btDefaultMotionState* motion_state =
        new btDefaultMotionState(bt_transform);

    // Create rigid body
    btRigidBody::btRigidBodyConstructionInfo floor_rb_info(
        mass, motion_state, self->collision_shape, local_inertia
    );
    self->rigid_body = new btRigidBody(floor_rb_info);

    // Add rigid body to physics engine
    physics->dynamics_world->addRigidBody(self->rigid_body);

    return (collider_t*) self;
}

/* *********************************************************************
 * Static mesh collider construct
 * ********************************************************************/
collider_t* collider_create_mesh(
    glm::mat4 initial_transform,
    mesh_t* mesh,
    physics_t* physics
)
{
    collider_plane_t* self =
        (collider_plane_t*) malloc(sizeof(collider_plane_t));

    self->parent.type    = COLLIDER_TYPE_MESH;
    self->parent.physics = physics;

    btTriangleMesh* triangleMesh = new btTriangleMesh();

    // loops 3 indices at a time
    for (uint32_t i = 0; i < mesh->index_count; i += 3) {
        vertex_t* vertex_a = mesh->vertices + mesh->indices[i + 0];
        vertex_t* vertex_b = mesh->vertices + mesh->indices[i + 1];
        vertex_t* vertex_c = mesh->vertices + mesh->indices[i + 2];

        btVector3 a(
            vertex_a->position.x, vertex_a->position.y,
            vertex_a->position.z
        );
        btVector3 b(
            vertex_b->position.x, vertex_b->position.y,
            vertex_b->position.z
        );
        btVector3 c(
            vertex_c->position.x, vertex_c->position.y,
            vertex_c->position.z
        );

        // Add those 3 vertices to the mesh as a triagle
        triangleMesh->addTriangle(a, b, c, true);
    }

    // Create a btStaticPlaneShape
    self->collision_shape =
        new btBvhTriangleMeshShape(triangleMesh, true, true);
    self->collision_shape->setMargin(0.01f);

    // Add collision shape to physics engine
    physics->collision_shapes.push_back(self->collision_shape);

    // Bullet uses its own format to hold transform
    btTransform bt_transform;
    bt_transform.setFromOpenGLMatrix(glm::value_ptr(initial_transform));

    btDefaultMotionState* motion_state =
        new btDefaultMotionState(bt_transform);

    btRigidBody::btRigidBodyConstructionInfo rb_info(
        0, motion_state, self->collision_shape, btVector3(0, 0, 0)
    );
    self->rigid_body = new btRigidBody(rb_info);
    self->rigid_body->setCollisionFlags(
        self->rigid_body->getCollisionFlags() |
        btCollisionObject::CF_STATIC_OBJECT
    );
    // self->rigid_body->setRestitution(2.0f);

    // Add rigid body to physics engine
    physics->dynamics_world->addRigidBody(self->rigid_body);

    return (collider_t*) self;
}

/* *********************************************************************
 * Box collider construct (using mesh data)
 * ********************************************************************/
collider_t* collider_create_box_from_mesh(
    glm::mat4 initial_transform,
    mesh_t* mesh,
    physics_t* physics
)
{
    collider_box_t* self =
        (collider_box_t*) malloc(sizeof(collider_box_t));

    self->parent.type    = COLLIDER_TYPE_BOX;
    self->parent.physics = physics;

    // Calculate box parameters from mesh
    // (0.5 is because Bullet wants it this way)
    glm::vec3 bounding_box = mesh_calculate_bounding_box(mesh) * 0.5f;

    // Create collision shape from bounding box
    self->collision_shape = new btBoxShape(
        btVector3(bounding_box.x, bounding_box.y, bounding_box.z)
    );

    // Extract scale from initial_transform
    glm::vec3 scale;
    scale.x = glm::length(initial_transform[0]);
    scale.y = glm::length(initial_transform[1]);
    scale.z = glm::length(initial_transform[2]);

    // Scales collision shape
    self->collision_shape->setLocalScaling(
        btVector3(scale.x, scale.y, scale.z)
    );

    // Add collision shape to physics engine
    physics->collision_shapes.push_back(self->collision_shape);

    // Calculate inertia
    btScalar mass(1.0f);
    btVector3 local_inertia(0.0f, 0.0f, 0.0f);
    if (mass > 0.0f) {
        self->collision_shape->calculateLocalInertia(
            mass, local_inertia
        );
    }

    // Load transform into motion state
    btTransform bt_transform;
    bt_transform.setFromOpenGLMatrix(glm::value_ptr(initial_transform));
    btDefaultMotionState* motion_state =
        new btDefaultMotionState(bt_transform);

    // Create rigid body
    btRigidBody::btRigidBodyConstructionInfo rb_info(
        mass, motion_state, self->collision_shape, local_inertia
    );
    self->rigid_body = new btRigidBody(rb_info);

    // Add rigid body to physics engine
    physics->dynamics_world->addRigidBody(self->rigid_body);

    return (collider_t*) self;
}

/* *********************************************************************
 * Sphere collider constructor
 * ********************************************************************/
collider_t* collider_create_sphere(
    glm::mat4 initial_transform,
    float radius,
    physics_t* physics
)
{
    collider_box_t* self =
        (collider_box_t*) malloc(sizeof(collider_box_t));
    self->parent.type    = COLLIDER_TYPE_SPHERE;
    self->parent.physics = physics;

    // Extract scale from initial_transform
    glm::vec3 scale;
    scale.x = glm::length(initial_transform[0]);
    scale.y = glm::length(initial_transform[1]);
    scale.z = glm::length(initial_transform[2]);

    // Create collision shape by radius
    self->collision_shape = new btSphereShape(radius);

    // Use this as a storage for scaling
    // probably misusing Bullet here, but this local scaling,
    // is used in collider_update_transform()
    self->collision_shape->setLocalScaling(
        btVector3(scale.x, scale.y, scale.z)
    );

    // Add collision shape to physics engine
    physics->collision_shapes.push_back(self->collision_shape);

    // Create motion state
    btScalar mass(1.0f);  // must be not 0 with dynamics
    btVector3 local_inertia(0.0f, 0.0f, 0.0f);

    // When has mass add inertia
    self->collision_shape->calculateLocalInertia(mass, local_inertia);

    // Bullet uses its own format to hold transform
    btTransform bt_transform;
    bt_transform.setFromOpenGLMatrix(glm::value_ptr(initial_transform));
    btDefaultMotionState* motion_state =
        new btDefaultMotionState(bt_transform);

    // Create rigid body
    btRigidBody::btRigidBodyConstructionInfo rb_info(
        mass, motion_state, self->collision_shape, local_inertia
    );
    self->rigid_body = new btRigidBody(rb_info);

    // Add rigid body to physics engine
    physics->dynamics_world->addRigidBody(self->rigid_body);

    return (collider_t*) self;
}

/* *********************************************************************
 * Capsule collider contructor
 * ********************************************************************/
collider_t* collider_create_capsule(
    glm::mat4 initial_transform,
    float radius,
    float height,
    physics_t* physics
)
{
    collider_box_t* self =
        (collider_box_t*) malloc(sizeof(collider_box_t));
    self->parent.type    = COLLIDER_TYPE_SPHERE;
    self->parent.physics = physics;

    // Extract scale from initial_transform
    glm::vec3 scale;
    scale.x = glm::length(initial_transform[0]);
    scale.y = glm::length(initial_transform[1]);
    scale.z = glm::length(initial_transform[2]);

    // Create collision shape by radius
    self->collision_shape = new btCapsuleShape(radius, height);

    // Use this as a storage for scaling
    // probably misusing Bullet here, but this local scaling,
    // is used in collider_update_transform()
    self->collision_shape->setLocalScaling(
        btVector3(scale.x, scale.y, scale.z)
    );

    // Add collision shape to physics engine
    physics->collision_shapes.push_back(self->collision_shape);

    // Create motion state
    btScalar mass(1.0f);  // must be not 0 with dynamics
    btVector3 local_inertia(0.0f, 0.0f, 0.0f);

    // When has mass add inertia
    self->collision_shape->calculateLocalInertia(mass, local_inertia);

    // Bullet uses its own format to hold transform
    btTransform bt_transform;
    bt_transform.setFromOpenGLMatrix(glm::value_ptr(initial_transform));
    btDefaultMotionState* motion_state =
        new btDefaultMotionState(bt_transform);

    // Create rigid body
    btRigidBody::btRigidBodyConstructionInfo rb_info(
        mass, motion_state, self->collision_shape, local_inertia
    );
    self->rigid_body = new btRigidBody(rb_info);

    // Add rigid body to physics engine
    physics->dynamics_world->addRigidBody(self->rigid_body);

    return (collider_t*) self;
}

/* *********************************************************************
 * update transform based on physics changes
 * ********************************************************************/
glm::mat4
collider_update_transform(collider_t* self, glm::mat4 target_transform)
{
    if (self->type == COLLIDER_TYPE_BOX) {
        collider_box_t* impl = (collider_box_t*) self;

        btTransform trans;
        impl->rigid_body->getMotionState()->getWorldTransform(trans);

        // fills in changes to target_transform
        trans.getOpenGLMatrix(glm::value_ptr(target_transform));

        // Take local scaling back from collision shape.
        // And apply it on transform.
        // It does not seem that this local scaling is used by Bullet,
        // which feels strange.
        // local scaling is set in create_collider_X() functions
        // of this unit.
        // TODO: Learn if this is the right way of doing it
        btVector3 bt_scale = impl->collision_shape->getLocalScaling();
        target_transform[0] *= bt_scale.getX();
        target_transform[1] *= bt_scale.getY();
        target_transform[2] *= bt_scale.getZ();

        return target_transform;

    } else if (self->type == COLLIDER_TYPE_SPHERE) {
        collider_sphere_t* impl = (collider_sphere_t*) self;

        btTransform trans;
        impl->rigid_body->getMotionState()->getWorldTransform(trans);

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

    else if (self->type == COLLIDER_TYPE_MESH) {
        collider_mesh_t* impl = (collider_mesh_t*) self;

        btTransform trans;
        impl->rigid_body->getMotionState()->getWorldTransform(trans);

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

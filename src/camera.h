#pragma once

#include "shader.h"
#include "gl.h"
#include "camera.h"

#include <math.h>

#include "glm/ext.hpp"
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <math.h>

#include "control.h"

typedef struct
{
    glm::mat4 view_matrix;
    glm::mat4 camera_matrix;

    float speed;

    int width;
    int height;
} camera_t;

camera_t *camera_create(int width, int height, glm::vec3 position);

void camera_update_matrix(
    camera_t *self,
    float fov_deg,
    float near_plane,
    float far_plane);

void camera_look_at(camera_t *self, glm::vec3 target)
{
    self->view_matrix = glm::lookAt(
        glm::vec3(self->view_matrix[3]),
        target,
        glm::vec3(0.0f, 1.0f, 0.0f));
}

camera_t *camera_create(
    int width, int height, glm::vec3 position, glm::vec3 target)
{
    camera_t *self = (camera_t *)malloc(sizeof(camera_t));

    self->view_matrix = glm::translate(glm::mat4(1.0f), position);

    // Will calculate later
    self->camera_matrix = glm::mat4(1.0f);

    self->width = width;
    self->height = height;

    self->speed = 0.1f;

    camera_look_at(self, target);

    return self;
}

void camera_update_matrix(
    camera_t *self,
    float fov_deg,
    float near_plane,
    float far_plane)
{
    float aspect = (float)self->width / self->height;
    glm::mat4 projection = glm::perspective(
        glm::radians(fov_deg), aspect, near_plane, far_plane);

    // Sets new camera matrix
    self->camera_matrix = projection * self->view_matrix;
}

void moveMatrixToLocation(glm::mat4 &matrix, const glm::vec3 &location)
{
    // Extract the translation part (last column) of the matrix
    glm::vec3 currentLocation = glm::vec3(matrix[3]);

    // Calculate the translation vector to move the matrix to the specified location
    glm::vec3 translationVector = location - currentLocation;

    // Update the translation component of the matrix
    matrix = glm::translate(matrix, translationVector);
}

// *********************************************************************
// TODO void camera_handle_input(input_evt*
// *********************************************************************
void camera_handle_event(camera_t *self, uint8_t event)
{
    if (event == CONTROL_CAMERA_FORWARD)
    {
        glm::vec3 forward_direction = -glm::normalize(glm::vec3(
            self->camera_matrix[0][2],
            self->camera_matrix[1][2],
            self->camera_matrix[2][2]));
        self->view_matrix = glm::translate(
            self->view_matrix,
            forward_direction * self->speed);
    }
    if (event == CONTROL_CAMERA_BACK)
    {
        glm::vec3 backward_direction = glm::normalize(glm::vec3(
            self->camera_matrix[0][2],
            self->camera_matrix[1][2],
            self->camera_matrix[2][2]));
        // Update the camera's position
        self->view_matrix = glm::translate(
            self->view_matrix,
            backward_direction * self->speed);
    }
    if (event == CONTROL_CAMERA_LEFT)
    {
        glm::vec3 left_direction = glm::normalize(glm::vec3(
            self->camera_matrix[0][0],
            self->camera_matrix[1][0],
            self->camera_matrix[2][0]));
        self->view_matrix = glm::translate(
            self->view_matrix,
            left_direction * self->speed);
    }

    if (event == CONTROL_CAMERA_RIGHT)
    {
        glm::vec3 right_direction = -glm::normalize(glm::vec3(
            self->camera_matrix[0][0],
            self->camera_matrix[1][0],
            self->camera_matrix[2][0]));
        self->view_matrix = glm::translate(
            self->view_matrix,
            right_direction * self->speed);
    }
    if (event == CONTROL_CAMERA_UP)
    {
        glm::vec3 up_direction = -glm::normalize(glm::vec3(
            self->camera_matrix[0][1],
            self->camera_matrix[1][1],
            self->camera_matrix[2][1]));
        self->view_matrix = glm::translate(
            self->view_matrix,
            up_direction * self->speed);
    }
    if (event == CONTROL_CAMERA_DOWN)
    {
        glm::vec3 down_direction = glm::normalize(glm::vec3(
            self->camera_matrix[0][1],
            self->camera_matrix[1][1],
            self->camera_matrix[2][1]));
        self->view_matrix = glm::translate(
            self->view_matrix,
            down_direction * self->speed);
    }
    if (event == CONTROL_CAMERA_PITCH_UP)
    {
        // Create a rotation matrix for pitch around the camera's right (X) axis
        glm::mat4 pitch_rotation = glm::rotate(
            glm::mat4(1.0f),
            -glm::radians(1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f));
        self->view_matrix = pitch_rotation * self->view_matrix;
    }
    if (event == CONTROL_CAMERA_PITCH_DOWN)
    {
        // Create a rotation matrix for pitch around the camera's right (X) axis
        glm::mat4 pitch_rotation = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(1.0f),
            glm::vec3(1.0f, 0.0f, 0.0f));
        self->view_matrix = pitch_rotation * self->view_matrix;
    }
    if (event == CONTROL_CAMERA_YAW_LEFT)
    {
        glm::mat4 yaw_rotation = glm::rotate(
            glm::mat4(1.0f),
            -glm::radians(1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        self->view_matrix = yaw_rotation * self->view_matrix;
    }
    if (event == CONTROL_CAMERA_YAW_RIGHT)
    {
        glm::mat4 yaw_rotation = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        self->view_matrix = yaw_rotation * self->view_matrix;
    }
    if (event == CONTROL_CAMERA_ROLL_LEFT)
    {
        glm::mat4 roll_rotation = glm::rotate(
            glm::mat4(1.0f),
            -glm::radians(1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f));
        self->view_matrix = roll_rotation * self->view_matrix;
    }
    if (event == CONTROL_CAMERA_ROLL_RIGHT)
    {
        // Create a rotation matrix for yaw
        // around the world's up (Z) axis
        glm::mat4 roll_rotation = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f));
        self->view_matrix = roll_rotation * self->view_matrix;
    }
}

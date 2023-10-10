#pragma once

#include "gl.h"
#include "camera.h"

#define CONTROL_MODE_CAMERA 1
#define CONTROL_MODE_PC     2

#define CONTROL_CAMERA_FORWARD      1
#define CONTROL_CAMERA_BACK         2
#define CONTROL_CAMERA_LEFT         3
#define CONTROL_CAMERA_RIGHT        4
#define CONTROL_CAMERA_UP           5
#define CONTROL_CAMERA_DOWN         6
#define CONTROL_CAMERA_PITCH_UP     7
#define CONTROL_CAMERA_PITCH_DOWN   8
#define CONTROL_CAMERA_ROLL_LEFT    9
#define CONTROL_CAMERA_ROLL_RIGHT  10
#define CONTROL_CAMERA_YAW_LEFT    11
#define CONTROL_CAMERA_YAW_RIGHT   12

// *********************************************************************
// Control struct
// *********************************************************************

typedef struct control_t {
    GLFWwindow* window;
    uint8_t mode;
} control_t;

// *********************************************************************
// Create control
// *********************************************************************

control_t* control_create(GLFWwindow* window, uint8_t mode) {
    control_t* self = (control_t*) malloc(sizeof(control_t));

    self->window = window;
    self->mode   = mode;

    return self;
}

// *********************************************************************
// Create control
// *********************************************************************

uint8_t control_get_event(control_t* self)
{
    if (self->mode == CONTROL_MODE_CAMERA) {

        if (glfwGetKey(self->window, GLFW_KEY_W) == GLFW_PRESS) {
            return CONTROL_CAMERA_FORWARD;
        }
        if (glfwGetKey(self->window, GLFW_KEY_S) == GLFW_PRESS) {
            return CONTROL_CAMERA_BACK;
        }
        if (glfwGetKey(self->window, GLFW_KEY_A) == GLFW_PRESS) {
            return CONTROL_CAMERA_LEFT;
        }
        if (glfwGetKey(self->window, GLFW_KEY_D) == GLFW_PRESS) {
            return CONTROL_CAMERA_RIGHT;
        }
        if (glfwGetKey(self->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            return CONTROL_CAMERA_UP;
        }
        if (glfwGetKey(self->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            return CONTROL_CAMERA_DOWN;
        }
        if (glfwGetKey(self->window, GLFW_KEY_UP) == GLFW_PRESS) {
            return CONTROL_CAMERA_PITCH_UP;
        }
        if (glfwGetKey(self->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            return CONTROL_CAMERA_PITCH_DOWN;
        }
        if (glfwGetKey(self->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            return CONTROL_CAMERA_YAW_LEFT;
        }
        if (glfwGetKey(self->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            return CONTROL_CAMERA_YAW_RIGHT;
        }
        if (glfwGetKey(self->window, GLFW_KEY_COMMA) == GLFW_PRESS) {
            return CONTROL_CAMERA_ROLL_LEFT;
        }
        if (glfwGetKey(self->window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
            return CONTROL_CAMERA_ROLL_RIGHT;
        }
    }

    return NULL;
}



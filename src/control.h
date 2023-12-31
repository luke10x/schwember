#pragma once

#include "camera.h"
#include "gl.h"

#define CONTROL_MODE_CAMERA       1
#define CONTROL_MODE_PC           2

// Floating camera controls
#define CONTROL_CAMERA_FORWARD    1
#define CONTROL_CAMERA_BACK       2
#define CONTROL_CAMERA_LEFT       3
#define CONTROL_CAMERA_RIGHT      4
#define CONTROL_CAMERA_UP         5
#define CONTROL_CAMERA_DOWN       6
#define CONTROL_CAMERA_PITCH_UP   7
#define CONTROL_CAMERA_PITCH_DOWN 8
#define CONTROL_CAMERA_ROLL_LEFT  9
#define CONTROL_CAMERA_ROLL_RIGHT 10
#define CONTROL_CAMERA_YAW_LEFT   11
#define CONTROL_CAMERA_YAW_RIGHT  12

// Piece controls
#define CONTROL_PC_FORWARD        21
#define CONTROL_PC_BACK           22
#define CONTROL_PC_LEFT           23
#define CONTROL_PC_RIGHT          24

// More piece controls
#define CONTROL_PC_SET_STANDARD_SHADER 31
#define CONTROL_PC_SET_WEIGHTS_SHADER  32

/* *********************************************************************
 * Control
 * ********************************************************************/
typedef struct control_t {
    GLFWwindow* window;
    uint8_t mode;
} control_t;

/* *********************************************************************
 * Create control
 * ********************************************************************/
control_t* control_create(GLFWwindow* window, uint8_t mode)
{
    control_t* self = (control_t*) malloc(sizeof(control_t));

    self->window = window;
    self->mode   = mode;

    return self;
}

/* *********************************************************************
 * gets event (that occured)
 * ********************************************************************/
bool tab_pressed = false;

uint8_t control_get_event(control_t* self)
{
    // Toggle mode
    if (glfwGetKey(self->window, GLFW_KEY_TAB) == GLFW_PRESS) {
        tab_pressed = true;
    } else {
        if (tab_pressed) {
            tab_pressed = false;
            if (self->mode == CONTROL_MODE_CAMERA) {
                self->mode = CONTROL_MODE_PC;
                return 0;
            }
            if (self->mode == CONTROL_MODE_PC) {
                self->mode = CONTROL_MODE_CAMERA;
                return 0;
            }
        }
    }
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
        if (glfwGetKey(self->window, GLFW_KEY_LEFT_SHIFT) ==
            GLFW_PRESS) {
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

    if (self->mode == CONTROL_MODE_PC) {
        if (glfwGetKey(self->window, GLFW_KEY_W) == GLFW_PRESS) {
            return CONTROL_PC_FORWARD;
        }
        if (glfwGetKey(self->window, GLFW_KEY_S) == GLFW_PRESS) {
            return CONTROL_PC_BACK;
        }
        if (glfwGetKey(self->window, GLFW_KEY_A) == GLFW_PRESS) {
            return CONTROL_PC_LEFT;
        }
        if (glfwGetKey(self->window, GLFW_KEY_D) == GLFW_PRESS) {
            return CONTROL_PC_RIGHT;
        }
    }

    return 0;
}

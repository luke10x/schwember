#pragma once

#include "shader.h"

#include "glm/ext.hpp"
#include <glm/gtx/vector_angle.hpp>

#include <math.h>

typedef struct {
  glm::vec3 location;
  glm::vec3 yaw;
  glm::vec3 pitch;

  glm::mat4 camera_matrix;

	// Prevents the camera from jumping around when first clicking left click
	int firstClick;

  int width;
  int height;

  float speed;
  float sensitivity;

} camera_t;

camera_t* camera_create(int width, int height, glm::vec3 position);

void camera_update_matrix(
  camera_t* self,
  float fov_deg,
  float near_plane,
  float far_plane
);

void camera_matrix(
  camera_t* self,
  shader_t* shader,
  const char* uniform
);

void camera_inputs(camera_t* self, GLFWwindow* window);


// implementatation


#include <math.h>

#include "gl.h"

#include "camera.h"
#include "shader.h"


camera_t* camera_create(int width, int height, glm::vec3 position) {
  camera_t* self = (camera_t*)malloc(sizeof(camera_t));

  self->location  = position;
  self->yaw       = glm::vec3(0.0f, 0.0f, -1.0f); // look around left/right directions
  self->pitch     = glm::vec3(0.0f, 1.0f, 0.0f);  // look up/down

  self->camera_matrix = glm::mat4(1.0f);

  self->width  = width;
  self->height = height;

  self->firstClick = 0;

  self->speed       = 0.1f;
  self->sensitivity = 100.0f;

  return self;
}

void camera_update_matrix(
  camera_t* self,
  float fov_deg,
  float near_plane,
  float far_plane
) {
  float aspect = (float) self->width / self->height;

	// Initializes identity matrixes
	glm::mat4 view       = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
  glm::vec3 eye    = self->location;
  glm::vec3 center = self->location + self->yaw;
	view = glm::lookAt(eye, center, self->pitch);

	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(fov_deg), aspect, near_plane, far_plane);

	// Sets new camera matrix
	self->camera_matrix = projection * view;
}

void camera_matrix(
  camera_t* self,
  shader_t* shader,
  const char* uniform
) {
  GLuint location = glGetUniformLocation(shader->ID, uniform);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(self->camera_matrix));
}

void camera_inputs(camera_t* self, GLFWwindow* window) {
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    self->location += self->yaw * self->speed;;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    self->location += glm::normalize(glm::cross(self->yaw, self->pitch)) * -self->speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    glm::vec3 tempVec = self->yaw * -self->speed;
    self->location += tempVec;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    self->location += glm::normalize(glm::cross(self->yaw, self->pitch)) * self->speed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    self->location += self->pitch * self->speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    self->location -= self->pitch * self->speed;
	}

  glm::vec3 orientation_copy = self->yaw;
  glm::vec3 crossed = glm::cross(self->yaw, self->pitch);

    // Normalize the resulting vector
  glm::vec3 normalized = glm::normalize(crossed);


  float rot_x = 0.0f;
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    rot_x = 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    rot_x = -1.0f;
  }

  if (rot_x != 0.0f) {
    float angle = glm::angle(normalized, self->pitch);
    glm::vec3 new_orientation = glm::rotate(orientation_copy, glm::radians(-rot_x), normalized);
		if (fabsf(angle - glm::radians(90.0f)) <= glm::radians(85.0f)) {
      self->yaw = new_orientation;
		}
  }

  float rot_y = 0.0f;
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    rot_y = 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    rot_y = -1.0f;
  }

  if (rot_y != 0.0f) {
    // Rotates the Orientation left and right
    self->yaw = glm::rotate(self->yaw, glm::radians(rot_y), self->pitch);
  }

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		self->speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		self->speed = 0.1f;
	}

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (self->firstClick)
		{
			glfwSetCursorPos(window, (self->width / 2), (self->height / 2));
			self->firstClick = 0;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = self->sensitivity * (float)(mouseY - (self->height / 2)) / self->height;
		float rotY = self->sensitivity * (float)(mouseX - (self->width / 2))  / self->width;

		// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(
      self->yaw,
      glm::radians(-rotX),
      glm::normalize(glm::cross(self->yaw, self->pitch))
    );

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, self->pitch) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			self->yaw = newOrientation;
		}

		// Rotates the Orientation left and right
		self->yaw = glm::rotate(self->yaw, glm::radians(-rotY), self->pitch);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (self->width / 2), (self->height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		self->firstClick = true;
	}
}

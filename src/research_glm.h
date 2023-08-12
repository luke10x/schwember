#pragma once

#include <iostream>
#include <iomanip> 

#define GLM_FORCE_PURE

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

// Custom function to print glm::mat4 matrices with indentation
void printMatrix(const glm::mat4& matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << std::setw(8) << std::fixed << std::setprecision(2) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void printMatrixTest() {
    std::string message = "PrintMatrix() test";
    std::cout << message << std::endl;

    // Sample values for Translate and Rotate
  float Translate = 5.0f; // Translation along the negative z-axis
  glm::vec2 Rotate = glm::vec2(glm::radians(30.0f), glm::radians(45.0f)); // Rotation angles in radians

	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View       = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model      = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

  glm::mat4 result = Projection * View * Model;

  printMatrix(result);
}
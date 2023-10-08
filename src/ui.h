#pragma once

#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "pc.h"


const ImVec4 buttonNormalColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
const ImVec4 UI_BUTTON_ACTIVE_COLOR = ImVec4(0.7f, 0.2f, 0.2f, 1.0f);

// *********************************************************************
// Constant for selected mode
// --------------------------
// Different all controlls should be handled differently in those modes
// *********************************************************************
#define UI_MODE_CAMERA 1
#define UI_MODE_PC     2

// *********************************************************************
// Structure for (IMG)UI data
// *********************************************************************
typedef struct {
    uint8_t mode;
} ui_t;

// *********************************************************************
// Private functions defined at the bottom of this file               
// (But listed here)
// *********************************************************************
void ImGuiMatrix(const char* label, const glm::mat4& matrix);

// *********************************************************************
// Create UI
// ---------
// For now it just instantiates empty the structure
// *********************************************************************
ui_t* ui_create()
{
  ui_t* self = (ui_t*) malloc(sizeof(ui_t)); 

  self->mode = UI_MODE_CAMERA;
  return self;
}

// *********************************************************************
// Render UI
// *********************************************************************
void ui_draw(ui_t* self, pc_t* pc) {
  // ImGui render
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  // Your ImGui code here
  ImGui::Begin("Debug panel");

  ImGui::Text("Control: ");

  // They have to be precached because original state of the condition
  // has to be invariant across the duration of this iteration
  bool is_mode_camera = self->mode == UI_MODE_CAMERA;
  bool is_mode_pc     = self->mode == UI_MODE_PC;

  // Camera mode button
  if (is_mode_camera) {
    ImGui::PushStyleColor(ImGuiCol_Button, UI_BUTTON_ACTIVE_COLOR);
  }
  ImGui::SameLine();
  if (ImGui::Button("camera")) {
    self->mode = UI_MODE_CAMERA;
  }
  if (is_mode_camera) {
    ImGui::PopStyleColor();
  }

  // PC mode button
  if (is_mode_pc) {
    ImGui::PushStyleColor(ImGuiCol_Button, UI_BUTTON_ACTIVE_COLOR);
  }
  ImGui::SameLine();
  if (ImGui::Button("PC")) {
    self->mode = UI_MODE_PC;
  }
  if (is_mode_pc) {
    ImGui::PopStyleColor();
  }



/*
  */

  ImGuiMatrix("pc->transform", pc->transform);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// *********************************************************************
// Private: draws matrix
// *********************************************************************
void ImGuiMatrix(const char* label, const glm::mat4& matrix) {
    ImGui::Text("%s", label);
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            ImGui::Text("%f", matrix[col][row]);
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }
}


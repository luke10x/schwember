#pragma once

#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "pc.h"
#include "control.h"

const ImVec4 UI_BUTTON_ACTIVE_COLOR = ImVec4(0.7f, 0.2f, 0.2f, 1.0f);

// *********************************************************************
// Structure for (IMG)UI data
// *********************************************************************

typedef struct {
    control_t* control;
} ui_t;

// *********************************************************************
// Private functions defined at the bottom of this file               
// ----------------------------------------------------
// (But listed here)
// *********************************************************************

void ImGuiMatrix(const char* label, const glm::mat4& matrix);

// *********************************************************************
// Create UI
// ---------
// For now it just instantiates empty the structure
// *********************************************************************

ui_t* ui_create(control_t* control)
{
  ui_t* self = (ui_t*) malloc(sizeof(ui_t)); 

  self->control = control;
  return self;
}

// *********************************************************************
// Render UI
// *********************************************************************

void ui_draw(ui_t* self, pc_t* pc)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Debug panel");
  
    ImGui::Text("Control: ");
  
    // self->control.mode will be mutated therefore define variables
    // to remember it's original value
    bool is_mode_camera = self->control->mode == CONTROL_MODE_CAMERA;
    bool is_mode_pc     = self->control->mode == CONTROL_MODE_PC;
  
    // Camera mode button
    if (is_mode_camera) {
        ImGui::PushStyleColor(ImGuiCol_Button, UI_BUTTON_ACTIVE_COLOR);
    }
    ImGui::SameLine();
    if (ImGui::Button("camera")) {
        self->control->mode = CONTROL_MODE_CAMERA;
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
        self->control->mode = CONTROL_MODE_PC;
    }
    if (is_mode_pc) {
        ImGui::PopStyleColor();
    }
  
    ImGuiMatrix("pc->transform", pc->transform);
    ImGui::End();
  
    // Finally flush all here
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// *********************************************************************
// Private: draws matrix
// *********************************************************************

void ImGuiMatrix(const char* label, const glm::mat4& matrix)
{
    ImGui::Text("%s", label);
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            ImGui::Text("%f", matrix[col][row]);
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }
}


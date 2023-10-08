#pragma once

#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

#include "pc.h"

typedef struct {
} ui_t;

////////////////////////////////////////////////////////////////////////
// Private functions defined at the bottom of this file               
// (But listed here)
////////////////////////////////////////////////////////////////////////
void ImGuiMatrix(const char* label, const glm::mat4& matrix);

////////////////////////////////////////////////////////////////////////
// Create UI                                                          //
////////////////////////////////////////////////////////////////////////
ui_t* ui_create()
{
  ui_t* self = (ui_t*) malloc(sizeof(ui_t)); 
  // nothing for now
  return self;
}

////////////////////////////////////////////////////////////////////////
// Render UI                                                          //
////////////////////////////////////////////////////////////////////////
void ui_draw(ui_t* self, pc_t* pc) {

  // ImGui render
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  // Your ImGui code here
  ImGui::Begin("Hello, world!");
  ImGui::Text("This is a simple ImGui application.");
  ImGuiMatrix("pc->transform", pc->transform);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

////////////////////////////////////////////////////////////////////////
// Private: draws matrix                                              //
////////////////////////////////////////////////////////////////////////
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


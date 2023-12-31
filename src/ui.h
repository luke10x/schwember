#pragma once

#include <glm/glm.hpp>

#include "control.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "pc.h"
#include "ui_gltf_tree.h"

const ImVec4 UI_BUTTON_ACTIVE_COLOR = ImVec4(0.7f, 0.2f, 0.2f, 1.0f);

/* *********************************************************************
 * UI displays Dear ImGUI panels
 * ********************************************************************/
typedef struct {
    control_t* control;
    pc_t* pc;
    ui_gltf_tree_t* pc_glb_tree;
    int animation_frame;
} ui_t;

/* *********************************************************************
 * Private functions used in the next function but defined after
 * ********************************************************************/
void ImGuiMatrix(const glm::mat4& matrix);
void ImGuiMatrix3(const glm::mat4& matrix);

/* *********************************************************************
 * UI constructor
 * ********************************************************************/
ui_t* ui_create(control_t* control, pc_t* pc)
{
    ui_t* self = (ui_t*) malloc(sizeof(ui_t));

    self->pc = pc;
    self->pc_glb_tree =
        ui_gltf_tree_create(self->pc->glb->_data, self->pc);
    self->animation_frame = 50;

    self->control = control;
    return self;
}

/* *********************************************************************
 * Draw UI
 * ********************************************************************/
void ui_draw(ui_t* self, pc_t* pc)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Main setup");
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
    ImGui::End();

    ImGui::Begin("Piece:");
    ImGuiMatrix(pc->transform);
    ImGui::Text(
        "Mass: %+08.3fkg",
        ((collider_box_t*) pc->collider)->rigid_body->getMass()
    );

    static const char* items[2] = {"Standard shader", "Weights shader"};
    static const int event_codes[2] = {
        CONTROL_PC_SET_STANDARD_SHADER, CONTROL_PC_SET_WEIGHTS_SHADER
    };
    static int selected_option = 0;

    if (ImGui::BeginCombo("##combo", items[selected_option])) {
        for (int i = 0; i < 2; i++) {
            bool is_selected = (selected_option == i);
            if (ImGui::Selectable(items[i], is_selected)) {
                selected_option = i;
                printf("Selected %d\n", event_codes[selected_option]);
                pc_handle_event(self->pc, event_codes[selected_option]);
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    // Inside your ImGui window or ImGui frame:
    if (ImGui::SliderInt(
            "Slider Label", &(self->animation_frame), 0, 100
        )) {
        // This block is executed when the slider value changes
        // You can handle the updated value here
    }
    ImGui::End();

    ui_gltf_tree_draw(self->pc_glb_tree);

    // Finally flush all here
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/* *********************************************************************
 * Private: draws 4x4 matrix
 * ********************************************************************/
void ImGuiMatrix(const glm::mat4& matrix)
{
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            ImGui::Text("%+08.3f", matrix[col][row]);
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }
}

/* *********************************************************************
 * Private: draws 3x3 matrix
 * ********************************************************************/
void ImGuiMatrix3(const glm::mat3& matrix)
{
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            ImGui::Text("%+08.3f", matrix[col][row]);
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }
}

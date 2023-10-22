#pragma once

#include <stdlib.h>

#ifndef CGLTF_IMPLEMENTATION
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#endif

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "pc.h"

typedef struct ui_gltf_tree_t {
    cgltf_node* root_node;
    pc_t* pc;
    cgltf_node* selected_node;
    int current;
} ui_gltf_tree_t;

ui_gltf_tree_t* ui_gltf_tree_create(
    cgltf_data* glb_data,
    pc_t* pc  // TODO use event que instead
)
{
    ui_gltf_tree_t* self =
        (ui_gltf_tree_t*) malloc(sizeof(ui_gltf_tree_t));

    self->pc = pc;

    // Detects root node, or exits
    cgltf_node* root_node = NULL;
    for (int i = 0; i < glb_data->nodes_count; i++) {
        cgltf_node* node = glb_data->nodes + i;
        if (node->parent == NULL) {
            root_node = node;
        }
    }
    if (root_node == NULL) {
        /*
        fprintf(
            stderr, "Root node not found in the list of %d nodes\n",
            glb_data->nodes_count
        );
        */
        exit(1);
    }

    self->root_node     = root_node;
    self->selected_node = NULL;

    return self;
}

/* *********************************************************************
 * Private use for only (declare only, defined later)
 * ********************************************************************/
void recursive_node_tree(ui_gltf_tree_t* self, cgltf_node* node);

void sprintf_node_title(
    char* result_buffer,
    size_t buffer_size,
    cgltf_node* node,
    int count
);

/* *********************************************************************
 * Draw function
 * ********************************************************************/
void ui_gltf_tree_draw(ui_gltf_tree_t* self)
{
    ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("GLB node tree");

    ImGui::Separator();

    ImGui::Columns(2);
    self->current = 0;
    recursive_node_tree(self, self->root_node);

    ImGui::NextColumn();
    if (self->selected_node == NULL) {
        ImGui::Text("nothing selected");
    } else {
        char node_title[64];
        sprintf_node_title(
            node_title, sizeof(node_title), self->selected_node,
            self->current
        );

        ImGui::Text("Node %s:", node_title);
        if (self->selected_node->skin != NULL) {
            ImGui::Text("Skin is present");
        }
        if (self->selected_node->weights_count > 0) {
            ImGui::Text(
                "Weights count %zu", self->selected_node->weights_count
            );
        }
        if (self->selected_node->mesh != NULL) {
            ImGui::Text("Mesh is present");
        }
    }
    ImGui::Columns(1);

    ImGui::End();
}

/* *********************************************************************
 * Private: Recursively display all nodes
 * ********************************************************************/
void recursive_node_tree(ui_gltf_tree_t* self, cgltf_node* node)
{
    self->current++;
    bool isSelected = (node == self->selected_node);

    if (isSelected) {
        ImGui::PushStyleColor(
            ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)
        );  // Yellow text color
    }

    // Because node_title cannot be null, but in some files it is
    char node_title[64];
    sprintf_node_title(
        node_title, sizeof(node_title), node, self->current
    );

    bool is_expanded = ImGui::TreeNode(node_title);
    if (isSelected) {
        ImGui::PopStyleColor();  // Restore the text color
    }

    if (is_expanded) {
        if (node->children_count > 0) {
            for (int i = 0; i < node->children_count; i++) {
                cgltf_node* child = node->children[i];
                recursive_node_tree(self, child);
            }
        }
        ImGui::TreePop();
    } else if (ImGui::IsItemClicked(0)) {
        self->selected_node = node;

        // pointer difference will be selected bone index
        // I wonder why root node is the last though
        // but I aceepti it is what it is
        int idx = self->root_node - node;

        if (idx < 0) {
            fprintf(
                stderr, "Somehox selected bone is outside nodes list"
            );
        }
        self->pc->selected_joint_index = self->current;
    }
}

void sprintf_node_title(
    char* result_buffer,
    size_t buffer_size,
    cgltf_node* node,
    int count
)
{
    if (node->name) {
        // If node->name is not NULL, copy it to the output buffer
        snprintf(
            result_buffer, buffer_size, "%d) %s", count, node->name
        );
    } else {
        // If node->name is NULL, create a string representation using
        // the node's address
        snprintf(
            result_buffer, buffer_size, "%d) Node %p", count,
            (void*) node
        );
    }
}

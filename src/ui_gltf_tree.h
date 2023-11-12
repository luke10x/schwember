#pragma once

#include <stdlib.h>
#include <stdio.h>

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

void ListBonesAndIDs(const cgltf_data* data)
{
    for (cgltf_int i = 0; i < data->skins_count; ++i) {
        const cgltf_skin* skin = data->skins + i;

        // Iterate through the joints of the skin
        for (cgltf_int j = 0; j < skin->joints_count; ++j) {
            const cgltf_node* node = skin->joints[j];

            // Check if the boneNode represents a bone
            if (node) {
                // Print the bone's name (if available) and its joint ID
                // printf(
                //     "Bone Name: %s, Joint ID: %d\n",
                //     node->name ? node->name : "Unnamed", j
                // );
            }
            /*
            // this block does some more diagnostics with children order
            for (int k = 0; k < node->children_count; k++) {
                const cgltf_node* child = node->children[k];

                char child_name[128];
                sprintf(child_name, "%s", child->name);
                // printf("    %d child: %s\n", j, child_name);
            }
            */
        }
    }
    
    for (int i = 0; i < data->animations_count; i++) {
        cgltf_animation* animation = data->animations + i;
        // printf("Animation: %s\n", animation->name);
        for (int j = 0; j < animation->channels_count; j++) {
            cgltf_animation_channel* channel = animation->channels + j;
            cgltf_animation_sampler* sampler = channel->sampler;
            cgltf_accessor* time_accessor    = sampler->input;
            cgltf_accessor* output_accessor  = sampler->output;
            char* output_name = output_accessor->name;
            cgltf_animation_path_type path_type = channel->target_path;
            
            const char* target_node_name = channel->target_node->name != NULL ? channel->target_node->name : "unnamed node";
            char* path_type_label = (char*) malloc(sizeof(char*) * 99);
            switch (path_type) {
                case cgltf_animation_path_type_invalid:
                    snprintf(path_type_label, 7, "invalid");
                    break;
                case cgltf_animation_path_type_translation:
                    snprintf(path_type_label, 7, "translation");
                    break;
                case cgltf_animation_path_type_rotation:
                    snprintf(path_type_label, 7, "rotation");
                    break;
                case cgltf_animation_path_type_scale:
                    snprintf(path_type_label, 5, "scale");
                    break;
                case cgltf_animation_path_type_weights:
                    snprintf(path_type_label, 5, "weights");
                    break;
                case cgltf_animation_path_type_max_enum:
                    snprintf(path_type_label, 4, "enum");
                    break;
            }

            float* buffer =
                    (float*) malloc(sizeof(float) * time_accessor->count);
            int float_count = cgltf_accessor_unpack_floats(
                time_accessor, buffer, time_accessor->count
            );


            float* out =
                    (float*) malloc(sizeof(float) * output_accessor->count);
            int out_count = cgltf_accessor_unpack_floats(
                output_accessor, out, output_accessor->count
            );

            // printf("  Channel: %d %s (%s) (%d)(%d) \n", j, target_node_name, path_type_label, float_count, out_count);

            for (int k = 0; k < out_count; k++) {
                // printf("    %3d|%.4f|%.4f|\n", k, buffer[k], out[k]);
            }
        } 
    }
    

    // size_t nodes_count = data->nodes_count;
    // for (int i = 0; i < data->nodes; i++) {
    //     const cgltf_node
    // }
}

/* *********************************************************************
 * UI GLFT tree widget constructor
 * ********************************************************************/
ui_gltf_tree_t* ui_gltf_tree_create(
    cgltf_data* glb_data,
    pc_t* pc  // TODO use event que instead
)
{
    ListBonesAndIDs(glb_data);
    ui_gltf_tree_t* self =
        (ui_gltf_tree_t*) malloc(sizeof(ui_gltf_tree_t));

    self->pc = pc;

    // Detects root node, or exits
    const cgltf_skin* first_skin = &glb_data->skins[0];
    self->root_node              = first_skin->joints[0];

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
    // Increase current index
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

        // Compensate increasion of current index
        self->pc->selected_joint_index = self->current - 1;
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

#pragma once

#include <iostream>


#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

#include "mesh.h"
#include "model.h"

void __apply_node_transformations(vertex_t* current_vertex, cgltf_node* current_node) {
  if (current_node == NULL) return;

  if (current_node->has_scale) {
    current_vertex->position.x *= current_node->scale[0];
    current_vertex->position.y *= current_node->scale[1];
    current_vertex->position.z *= current_node->scale[2];
  }

  if (current_node->has_rotation) {
    glm::quat glmRotationQuaternion = glm::quat(
      current_node->rotation[3],
      current_node->rotation[0],
      current_node->rotation[1],
      current_node->rotation[2]
    );

    glm::vec3 glm_vertex = glm::vec3(
      current_vertex->position.x,
      current_vertex->position.y,
      current_vertex->position.z
    );

    glm_vertex = glm::rotate(glmRotationQuaternion, glm_vertex);

    current_vertex->position.x = glm_vertex.x;
    current_vertex->position.y = glm_vertex.y;
    current_vertex->position.z = glm_vertex.z;
  }

  if (current_node->has_translation) {
    current_vertex->position.x += current_node->translation[0];
    current_vertex->position.y += current_node->translation[1];
    current_vertex->position.z += current_node->translation[2];
  }

  // Recurse to parents
  if (current_node->parent != NULL) {
    __apply_node_transformations(current_vertex, current_node->parent);
  }
}

// Macro to simplify attributes loading code
#define LOAD_ATTRIBUTE(accesor, numComp, dataType, dstPtr) \
{ \
    int n = 0; \
    dataType *buffer = (dataType *)accesor->buffer_view->buffer->data + accesor->buffer_view->offset/sizeof(dataType) + accesor->offset/sizeof(dataType); \
    for (unsigned int k = 0; k < accesor->count; k++) \
    {\
        for (int l = 0; l < numComp; l++) \
        {\
            dstPtr[numComp*k + l] = buffer[n + l];\
        }\
        n += (int)(accesor->stride/sizeof(dataType));\
    }\
}

mesh_t* model_load_from_file(model_t* model, const char *gltf_file_name) {
  cgltf_options options;
  std::memset(&options, 0, sizeof(cgltf_options));

  cgltf_data* data = nullptr;
  cgltf_result result = cgltf_parse_file(&options, gltf_file_name, &data);

  if (result != cgltf_result_success) {
      std::cerr << "Error loading glTF file: " << result << std::endl;
  }

  result = cgltf_load_buffers(&options, data, gltf_file_name);
  if (result != cgltf_result_success) {
      std::cerr << "Error loading glTF file (BUFFERS): " << result << std::endl;
      exit(1);
  }
  // Loading successful, process the data here
    
  // Iterate through meshes and print their names
  for (size_t i = 0; i < data->meshes_count; ++i) {
    cgltf_mesh* mesh = &data->meshes[i];

    // Finds node for current mesh
    cgltf_node* current_node = NULL;
    for (int node_ind = 0; node_ind < data->nodes_count; node_ind++) {
      cgltf_node* node = &(data->nodes[node_ind]);
      cgltf_mesh* node_mesh = node->mesh;
      if(node_mesh == mesh) {
        current_node = node;
      }
    }

    for (cgltf_size i_prim = 0; i_prim < mesh->primitives_count; i_prim++) {
      cgltf_primitive* primitive = &mesh->primitives[i_prim];
      
      cgltf_accessor* position_accessor = primitive->attributes[0].data;
      size_t num_vertices = position_accessor->count;
      vertex_t* vertices = (vertex_t*) malloc(num_vertices * sizeof(vertex_t));

      cgltf_material* material = primitive->material;
      const char* materialName = material ? material->name : "  No Material";

      texture_t* texture_of_primitive = NULL;
      
      cgltf_float base_color[3];
      base_color[0] = 0.0f;
      base_color[1] = 1.0f; // green
      base_color[0] = 0.0f;

      if (material != NULL) {

        // Check if the material has a base color factor.
        if (material->has_pbr_metallic_roughness) {
            cgltf_pbr_metallic_roughness* pbr = &material->pbr_metallic_roughness;
            
            // Access the albedo (base color) factor, which contains the RGB color.
            base_color[0] = pbr->base_color_factor[0];
            base_color[1] = pbr->base_color_factor[1];
            base_color[2] = pbr->base_color_factor[2];
        }
        
        cgltf_texture* texture = material->pbr_metallic_roughness.base_color_texture.texture;

        if (texture != NULL) {
          cgltf_image* image = texture->image;
          const char* image_name = image->name;
          const char* image_uri = image->uri;
          cgltf_image* cgltfImage = image;

          if (image->buffer_view->buffer->data != NULL) {
            unsigned char *data = (unsigned char*) malloc(cgltfImage->buffer_view->size);
            int offset = (int)cgltfImage->buffer_view->offset;
            int stride = (int)cgltfImage->buffer_view->stride? (int)cgltfImage->buffer_view->stride : 1;

            // Copy buffer data to memory for loading
            for (unsigned int i = 0; i < cgltfImage->buffer_view->size; i++) {
                data[i] = ((unsigned char *) cgltfImage->buffer_view->buffer->data)[offset];
                offset += stride;
            }
            int size = (int) cgltfImage->buffer_view->size;

            int width, height, bpp;
            stbi_set_flip_vertically_on_load(0);
            unsigned char* stbi = stbi_load_from_memory(data, size,  &width, &height, &bpp, 0);

            texture_of_primitive = texture_create(
              stbi, GL_TEXTURE0, width, height, bpp);
            free (data);
          }
        }
      }

      for (int i = 0; i < num_vertices; i++) {
        vertex_t* current_vertex = (vertex_t*) (&vertices[i]);
        current_vertex->color.x = base_color[0];
        current_vertex->color.y = base_color[1];
        current_vertex->color.z = base_color[2];
      }

      // For each primitive attribute:
      for (cgltf_size i_attr = 0; i_attr < primitive->attributes_count; i_attr++) {
        cgltf_attribute* attribute = &primitive->attributes[i_attr];
        cgltf_accessor* accessor = attribute->data;

        if (!accessor) { continue; }

        cgltf_int index = attribute->index;
        if (index != 0) { continue; }
              
        cgltf_size float_count = cgltf_accessor_unpack_floats(accessor, NULL, 0);
        float* buffer = (float *) malloc(sizeof(float) * float_count);
        float_count = cgltf_accessor_unpack_floats(accessor, &buffer[0], float_count);

        if (attribute->type == cgltf_attribute_type_position) {
          for(int i = 0; i < float_count; i += 3) {
            vertex_t* current_vertex = (vertex_t*) (&vertices[i / 3]);
            current_vertex->position.x = buffer[i + 0];
            current_vertex->position.y = buffer[i + 1];
            current_vertex->position.z = buffer[i + 2];
          }
        }

        if (attribute->type == cgltf_attribute_type_joints) {
                  
            printf("Joint Indices in %s::\n",  mesh->name);

            // Access the joint indices data
            const cgltf_accessor* accessor = attribute->data;
           
            const cgltf_component_type component_type = accessor->component_type;
            const int num_component_vals = cgltf_num_components(accessor->type);

            if (
              (
                component_type == cgltf_component_type_r_16u ||
                component_type == cgltf_component_type_r_8u
              ) &&
              num_component_vals == 4
            ) {
                printf("Joint Accessor found :\n"          
                    "component_type = %d\n"
                    "just type = %d\n" "\n",
                    accessor->component_type,
                    accessor->type
                );
                uint32_t i = 0;
                while (i < accessor->count) {
                    cgltf_uint joints[4];
                    cgltf_bool res = cgltf_accessor_read_uint(accessor, i, joints, 4);
                    if (res == 0) {
                        fprintf(stderr, "cannot read joints data\n"); exit(1);
                    } else {
                      printf("Joints component_type[%d]xs indexes for a vertice #%d = {%d, %d, %d, %d}\n",
                        component_type,
                        i,
                        joints[0],
                        joints[1],
                        joints[2],
                        joints[3]
                      );
                    }
                    //     verts[i].joints.components.joint_0 = @intCast(u16, joints[0]);
                    i++;
                }
            } else {
              fprintf(
                  stderr, "Strange joint accessor type, but continue\n"
                "component_type = %d\n"
                "just type = %d\n" "\n",
                accessor->component_type, accessor->type

              );
              exit(1); // actually we don't continue...
            }

        }

        if (attribute->type == cgltf_attribute_type_weights) {
            printf("Weights\n");
            const cgltf_accessor* accessor = attribute->data;

            const cgltf_component_type component_type = accessor->component_type;
            const uint32_t num_component_vals = cgltf_num_components(accessor->type);

            if (component_type == cgltf_component_type_r_32f and num_component_vals == 4) {


                uint32_t i = 0;
                while (i < accessor->count) {
                    cgltf_float weights[4];
                    cgltf_bool res = cgltf_accessor_read_float(accessor, i, weights, 4);
                    if (res == 0) {
                        fprintf(stderr, "cannot read joints data\n"); exit(1);
                    }                                    

                    printf("Weight for a vertice #%d = {%.4f, %.4f, %.4f, %.4f}\n",
                        i,
                        weights[0],
                        weights[1],
                        weights[2],
                        weights[3]
                    );
                    i++;
                }
            }
        }

        if (attribute->type == cgltf_attribute_type_normal) {
          for(int i = 0; i < float_count; i += 3) {
            vertex_t* current_vertex = (vertex_t*) (&vertices[i / 3]);
            current_vertex->normal.x = buffer[i + 0];
            current_vertex->normal.y = buffer[i + 1];
            current_vertex->normal.z = buffer[i + 2];
          }
        }

        if (attribute->type == cgltf_attribute_type_texcoord) {
          for(int i = 0; i < float_count; i += 2) {
            vertex_t* current_vertex = (vertex_t*) (&vertices[i / 2]);
            current_vertex->texUV.x = buffer[i + 0];
            current_vertex->texUV.y = buffer[i + 1];
          }
        }

        free(buffer);
      }

      // Apply transformations from node and its parents
      for (int i = 0; i < num_vertices; i++) {
        vertex_t* current_vertex = (vertex_t*) (&vertices[i]);
        __apply_node_transformations(current_vertex, current_node);
      }

      cgltf_accessor* index_accessor = primitive->indices;
      if (!index_accessor) {
        std::cerr << "Cannot find index accessor" << std::endl;
        exit(1);
      }
      
      size_t index_count = index_accessor->count;

      uint32_t* indices = (uint32_t*)malloc(sizeof(uint32_t) * index_count);
      for (cgltf_size i = 0; i < index_count; i++) {
          uint32_t index = cgltf_accessor_read_index(index_accessor, i);
          indices[i] = index;
      }

      int texture_count = (texture_of_primitive != NULL) ? 1 : 0;

      mesh_t* model_mesh = mesh_create(
        "named",
        vertices, num_vertices,
        indices, index_count,
        texture_of_primitive
      );
      
      model->meshes[model->num_meshes] = model_mesh;
      model->num_meshes++;
    }

    for (cgltf_size i = 0; i < data->materials_count; ++i) {
      cgltf_material* material = &data->materials[i];
      cgltf_float baseColorFactor[4];
      memcpy(baseColorFactor, material->pbr_metallic_roughness.base_color_factor, sizeof(baseColorFactor));

      cgltf_texture* texture = material->pbr_metallic_roughness.base_color_texture.texture;

      if (texture != NULL) {
          // Access texture information, e.g., index, name, or other properties
          // int textureIndex = texture->image_index;
          const char* textureName = texture->name;
          cgltf_image* image = texture->image;

          if (image != NULL) {
            const char* image_name = image->name;
            
            const cgltf_buffer_view* buffer_view = image->buffer_view;
            int image_index = texture->image - data->images;
  
            // printf("image name : %s (%d) \n ", image_name, image_index);
          }
      }
    }
  }

  // Iterate through animations and print their names
  // printf("Animations:\n");
  for (size_t i = 0; i < data->animations_count; ++i) {
    cgltf_animation* animation = &data->animations[i];
    // printf("  %zu: %s\n", i, animation->name ? animation->name : "Unnamed");
  }

  cgltf_free(data);

  return NULL;
}

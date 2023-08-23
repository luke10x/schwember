#pragma once

#include <iostream>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
// #include "cgltf/cgltf.h"

#include "mesh.h"

void printAccessorProperties(const cgltf_accessor* accessor, cgltf_data* data) {
    printf("Accessor Properties (%zu): \n", accessor->stride);
    printf("  Count: %zu\n", accessor->count);
    printf("  Type: %d\n", accessor->type);
    printf("  Component Type: %d\n", accessor->component_type);
}

void printSamplerData(cgltf_animation_sampler* sampler, cgltf_data* data) {
    printf("    Sampler Data:\n");

    // Print interpolation type
    const char* interpolationType = "";
    switch (sampler->interpolation) {
        case cgltf_interpolation_type_linear:
            interpolationType = "Linear";
            break;
        case cgltf_interpolation_type_step:
            interpolationType = "Step";
            break;
        case cgltf_interpolation_type_cubic_spline:
            interpolationType = "Cubic Spline";
            break;
        default:
            fprintf(stderr, "Unknown interpolation type");
            exit(0);
    }
    printf("      Interpolation Type: %s\n", interpolationType);

    // Access the input accessor's data
    // Access the input accessor's data
     printf("      Input Values:\n");
    for (size_t i = 0; i < sampler->input->count; ++i) {
        float inputValue;
        cgltf_bool success = cgltf_accessor_read_float(sampler->input, i, &inputValue, 1);
        if (success) {
            printf("          Time[%zu]: %f\n", i, inputValue);
        }
    }


    // Access the output accessor's data
    printf("      Output Values:\n");
    for (size_t i = 0; i < sampler->output->count; ++i) {
        float outputValue;
        cgltf_bool success = cgltf_accessor_read_float(sampler->output, i, &outputValue, 1);
        if (success) {
            printf("          Value[%zu]: %f\n", i, outputValue);
        }
    }

    printAccessorProperties(sampler->input, data);
    printAccessorProperties(sampler->output, data);
}
const char* getTargetPathName(cgltf_animation_channel* channel) {
    switch (channel->target_path) {
        case cgltf_animation_path_type_translation:
            return "Translation";
        case cgltf_animation_path_type_rotation:
            return "Rotation";
        case cgltf_animation_path_type_scale:
            return "Scale";
        case cgltf_animation_path_type_weights:
            return "Weights";
        default:
            return "Unknown";
    }
}static const unsigned char base64Table[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 62, 0, 0, 0, 63, 52, 53,
    54, 55, 56, 57, 58, 59, 60, 61, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 2, 3, 4,
    5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 0, 0, 0, 0, 0, 0, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51
};
static int GetSizeBase64(char *input)
{
    int size = 0;
    for (int i = 0; input[4*i] != 0; i++)
    {
        if (input[4*i + 3] == '=')
        {
            if (input[4*i + 2] == '=') size += 1;
            else size += 2;
        }
        else size += 3;
    }
    return size;
}
static unsigned char *DecodeBase64(char *input, int *size)
{
    *size = GetSizeBase64(input);
    unsigned char *buf = (unsigned char *)malloc(*size);
    for (int i = 0; i < *size/3; i++)
    {
        unsigned char a = base64Table[(int)input[4*i]];
        unsigned char b = base64Table[(int)input[4*i + 1]];
        unsigned char c = base64Table[(int)input[4*i + 2]];
        unsigned char d = base64Table[(int)input[4*i + 3]];
        buf[3*i] = (a << 2) | (b >> 4);
        buf[3*i + 1] = (b << 4) | (c >> 2);
        buf[3*i + 2] = (c << 6) | d;
    }
    if (*size%3 == 1)
    {
        int n = *size/3;
        unsigned char a = base64Table[(int)input[4*n]];
        unsigned char b = base64Table[(int)input[4*n + 1]];
        buf[*size - 1] = (a << 2) | (b >> 4);
    }
    else if (*size%3 == 2)
    {
        int n = *size/3;
        unsigned char a = base64Table[(int)input[4*n]];
        unsigned char b = base64Table[(int)input[4*n + 1]];
        unsigned char c = base64Table[(int)input[4*n + 2]];
        buf[*size - 2] = (a << 2) | (b >> 4);
        buf[*size - 1] = (b << 4) | (c >> 2);
    }
    return buf;
}
void printAnimationChannelInfo(cgltf_animation_channel* channel, size_t index) {
    const char* targetNodeName = channel->target_node ? channel->target_node->name : "Unnamed Node";
    const char* targetPath = getTargetPathName(channel);

    printf("  Channel %zu: Target Node: %s, Target Path: %s\n", index, targetNodeName, targetPath);
}

void applyAnimationTransform(cgltf_data* data, size_t animationIndex, double keyframeTimeInSeconds) {
    if (!data || animationIndex >= data->animations_count) {
        return;
    }

    cgltf_animation* animation = &data->animations[animationIndex];
    // double animationDuration = animation->duration;
    // double animationTimeInSeconds = fmod(keyframeTimeInSeconds, animationDuration);
    double animationTimeInSeconds = keyframeTimeInSeconds;

    for (size_t i = 0; i < animation->channels_count; ++i) {
        cgltf_animation_channel* channel = &animation->channels[i];
        printAnimationChannelInfo(channel, i);



        cgltf_animation_sampler* sampler = channel->sampler;
        printSamplerData(sampler, data);

        cgltf_node* targetNode = channel->target_node;

        // // Find the corresponding animation samples
        // float previousTime = 0.0f, nextTime = 0.0f;
        // float previousValue = 0.0f, nextValue = 0.0f;

        // for (size_t j = 0; j < sampler->input->count; ++j) {


        //     float time = ((float*)sampler->input->buffer_view->buffer->data)[j];
        // std::cerr << "Error sampler time: "  << std::endl;

        //     if (time <= animationTimeInSeconds) {
        //         previousTime = time;
        //         previousValue = ((float*)sampler->output->buffer_view->buffer->data)[j];
        //     } else {
        //         nextTime = time;
        //         nextValue = ((float*)sampler->output->buffer_view->buffer->data)[j];
        //         break;
        //     }
        // }

        // // Linear interpolation
        // float alpha = (animationTimeInSeconds - previousTime) / (nextTime - previousTime);
        // cgltf_float interpolatedValue = previousValue * (1.0f - alpha) + nextValue * alpha;

        // // Apply the transformation to the node's local matrix
        // if (targetNode != NULL) {
        //     cgltf_node_transform_local(targetNode, &interpolatedValue);
        // }
    }
}

mesh_t* load_mesh_from_file(const char *gltf_file_name) {
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
  printf("Meshes:\n");
  for (size_t i = 0; i < data->meshes_count; ++i) {
    cgltf_mesh* mesh = &data->meshes[i];
    printf("  %zu: %s\n", i, mesh->name ? mesh->name : "Unnamed");
    for (cgltf_size i_prim = 0; i_prim < mesh->primitives_count; i_prim++) {
      cgltf_primitive* primitive = &mesh->primitives[i_prim];
      
      cgltf_material* material = primitive->material;
      const char* materialName = material ? material->name : "  No Material";
      printf("   Material Name: %s\n", materialName);

      cgltf_texture* texture = material->pbr_metallic_roughness.base_color_texture.texture;
      cgltf_image* image = texture->image;

      const char* image_name = image->name;
      const char* image_uri = image->uri;
      cgltf_image* cgltfImage = image;

      if (image->buffer_view->buffer->data != NULL) {
        unsigned char *data = (unsigned char*) malloc(cgltfImage->buffer_view->size);
        int offset = (int)cgltfImage->buffer_view->offset;
        int stride = (int)cgltfImage->buffer_view->stride? (int)cgltfImage->buffer_view->stride : 1;

        // Copy buffer data to memory for loading
        for (unsigned int i = 0; i < cgltfImage->buffer_view->size; i++)
        {
            data[i] = ((unsigned char *)cgltfImage->buffer_view->buffer->data)[offset];
            offset += stride;
        }
        int size = (int)cgltfImage->buffer_view->size;

        int width, height, bpp;
        unsigned char* stbi = stbi_load_from_memory(data, size,  &width, &height, &bpp, 0);
                    
        printf("   Material Name: %s -- %s --  %d = %dx%dx%d\n", materialName, image_name, size, width, height, bpp);

        free (data);
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
          printf("P");
          int stride = 3;
          for (int i = 0; i < float_count; i++) {
            printf(" %.2f ", buffer[i]);
          }
          if (i % stride == 0) {
            printf(">> ");

          }
        
        }

        if (attribute->type == cgltf_attribute_type_normal) {
          printf("N");
          for (int i = 0; i < float_count; i++) {
            printf(" %.2f ", buffer[i]);
          }
        }
        if (attribute->type == cgltf_attribute_type_color) {
          printf("C");
        }
        if (attribute->type == cgltf_attribute_type_texcoord) {
          printf("X");
          for (int i = 0; i < float_count; i++) {
            printf(" %.2f ", buffer[i]);
          }
        }
        printf("\n");

        free(buffer);
      }
      
      cgltf_accessor* index_accessor = primitive->indices;
      if (!index_accessor) {
        std::cerr << "Cannot find index accessor" << std::endl;
        exit(1);
      }
      
      size_t index_count = index_accessor->count;


      uint32_t* indices = (uint32_t*)malloc(sizeof(uint32_t) * index_count);
      for (cgltf_size i = 0; i < index_count; i++) {
          uint32_t index = static_cast<uint32_t>(cgltf_accessor_read_index(index_accessor, i));
          indices[i] = index;
      }
      printf("IND: ");

      for (int i = 0; i < index_count; i++) {
        printf(" %d ", indices[i]);

      }

      printf("\n");


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
  
            printf("image name : %s (%d) \n ", image_name, image_index);
            

          }


          // ...
      }
          // Access texture information from the material
          // e.g., material->pbr_metallic_roughness.base_color_texture
    }
  }





            // Access vertex data here
            ////////////////////

    
    // printf("      Vertex Attributes:\n");
    // printf("      %-15s %-15s %-15s\n", "Attribute", "Type", "Values");

    // // Assume POSITION and NORMAL attributes are present
    // cgltf_attribute* positionAttribute = NULL;
    // cgltf_attribute* normalAttribute = NULL;

    // for (cgltf_size j = 0; j < primitive->attributes_count; ++j) {
    //     cgltf_attribute* attribute = &primitive->attributes[j];

    //     if (strcmp(attribute->name, "POSITION") == 0) {
    //         positionAttribute = attribute;
    //     } else if (strcmp(attribute->name, "NORMAL") == 0) {
    //         normalAttribute = attribute;
    //     }


    //            // Print attribute values        
    //            int numComponents = attribute->data->type == cgltf_type_scalar 
    //               ? 1 
    //               : attribute->data->type - cgltf_type_vec2 + 2;

    //     for (cgltf_size k = 0; k < attribute->data->count; ++k) {
    //         printf(" [");
    //         for (int l = 0; l < numComponents; ++l) {
    //             float value = 0.0f;
    //             cgltf_accessor_read_float(attribute->data, k * numComponents + l, &value, 1);
    //             printf(" %.2f", value);
    //         }
    //         printf(" ]");
    //     }
    // }

    // if (positionAttribute != NULL) {
    //     // Access and print position attribute
    //     // (Assuming it's a float vector with 3 components)
    //     // You can use positionAttribute->data to access the accessor
    //     // printf("POSITION       %-15s", positionAttribute->data->type);
    //     // Print position attribute values here
    //     printf("\n");
    // }

    // if (normalAttribute != NULL) {
    //     // Access and print normal attribute
    //     // (Assuming it's a float vector with 3 components)
    //     // You can use normalAttribute->data to access the accessor
    //     // printf("NORMAL         %-15s", normalAttribute->data->type);
    //     // Print normal attribute values here
    //     printf("\n");
    // }

    //         ////
    //     }
    // }

    // Iterate through animations and print their names
     // Iterate through animations and print their names
    printf("Animations:\n");
    for (size_t i = 0; i < data->animations_count; ++i) {
        cgltf_animation* animation = &data->animations[i];
        printf("  %zu: %s\n", i, animation->name ? animation->name : "Unnamed");
    }

    size_t animationIndex = 0;          // Replace with the desired animation index
    double keyframeTimeInSeconds = 1.0; // Replace with the desired keyframe time

    applyAnimationTransform(data, animationIndex, keyframeTimeInSeconds);

    // Free the loaded data when done
    std::cout << "Success " << result << std::endl;

    cgltf_free(data);

    return NULL;
}
#include <iostream>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

void printAccessorProperties(const cgltf_accessor* accessor, cgltf_data* data) {
    printf("Accessor Properties (%zu): \n", accessor->stride);
    printf("  Count: %zu\n", accessor->count);
    printf("  Type: %d\n", accessor->type);
    printf("  Component Type: %d\n", accessor->component_type);
    printf("  Buffer View Index: %zu\n", accessor->buffer_view);
    
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

void tinyGltfTest() {
    cgltf_options options;
    std::memset(&options, 0, sizeof(cgltf_options));

    cgltf_data* data = nullptr;
    cgltf_result result = cgltf_parse_file(&options, "assets/gltf/cubeman.glb", &data);
    // cgltf_result result = cgltf_parse_file(&options, "assets/skin.gltf", &data);
    if (result != cgltf_result_success) {
        std::cerr << "Error loading glTF file: " << result << std::endl;


    }
    // Loading successful, process the data here
    
        // Iterate through meshes and print their names
    printf("Meshes:\n");
    for (size_t i = 0; i < data->meshes_count; ++i) {
        cgltf_mesh* mesh = &data->meshes[i];
        printf("  %zu: %s\n", i, mesh->name ? mesh->name : "Unnamed");
    }

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
}
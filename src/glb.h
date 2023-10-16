#pragma once

#include <iostream>

#ifndef CGLTF_IMPLEMENTATION
#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#endif

typedef struct glb_t {
    cgltf_data* _data;
} glb_t;

glb_t* glb_create(const char* file_name)
{
    glb_t* self = (glb_t*) malloc(sizeof(glb_t));

    cgltf_options options;
    std::memset(&options, 0, sizeof(cgltf_options));

    self->_data = nullptr;
    cgltf_result result;

    // Parse GLB file
    result = cgltf_parse_file(&options, file_name, &(self->_data));
    if (result != cgltf_result_success) {
        fprintf(
            stderr, "Error loading GLB '%s' with cgltf_result[%d]\n",
            file_name, result
        );
        exit(1);
    }

    // Load buffers from the same GLB file into data
    result = cgltf_load_buffers(&options, self->_data, file_name);
    if (result != cgltf_result_success) {
        fprintf(
            stderr,
            "Error loading GLB  buffers from file "
            "'%s' with cgltf_result[%d]\n",
            file_name, result
        );
        exit(1);
    }
    return self;
}
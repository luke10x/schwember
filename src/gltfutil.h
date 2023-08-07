#include <iostream>

#define CGLTF_IMPLEMENTATION
#include <cgltf.h>


void tinyGltfTest() {
    cgltf_options options;
    std::memset(&options, 0, sizeof(cgltf_options));

    cgltf_data* data = nullptr;
    cgltf_result result = cgltf_parse_file(&options, "assets/gltf/cubeman.glb", &data);
    if (result == cgltf_result_success) {
        // Loading successful, process the data here

        std::cout << "Success " << result << std::endl;

        cgltf_free(data);
    } else {
        std::cerr << "Error loading glTF file: " << result << std::endl;
    }



}
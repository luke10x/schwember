#include "gpmutil.h"
#include "gltfutil.h"

#include <iostream>


// #include <iostream>
// #include <assimp/Importer.hpp>
// #include <assimp/scene.h>
// #include <assimp/postprocess.h>

int main() {

  printMatrixTest();

  tinyGltfTest();

    // Create an Assimp importer
    // Assimp::Importer importer;

    // // Load the GLTF file
    // const aiScene* scene = importer.ReadFile("../assets/gltf/cubeman.gltf", aiProcess_Triangulate | aiProcess_FlipUVs);

    // // Check if the import was successful
    // if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    //     std::cerr << "Error loading GLTF file: " << importer.GetErrorString() << std::endl;
    //     return 1;
    // }

    // Process the scene data, e.g., access meshes, materials, etc.

  return 0;
}
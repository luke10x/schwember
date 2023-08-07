cmake_minimum_required(VERSION 3.12)
project(MatrixTransform)

# Add the path to the src directory
include_directories(src)

# Set the path to the directory containing GLM headers
set(GLM_INCLUDE_DIR "/opt/homebrew/include")

# Use FetchContent to automatically download cgltf
include(FetchContent)
FetchContent_Declare(
  cgltf
  GIT_REPOSITORY https://github.com/jkuhlmann/cgltf.git
  GIT_TAG v1.13  # Adjust the version as needed
)

# Fetch cgltf at configure time
FetchContent_MakeAvailable(cgltf)

# Add the source file to the project
add_executable(MatrixTransform src/main.cpp)

# Include the GLM headers
# Include the Assimp headers
target_include_directories(MatrixTransform PRIVATE ${GLM_INCLUDE_DIR} ${cgltf_SOURCE_DIR})

# Set the C++ standard (if needed)
set(CMAKE_CXX_STANDARD 11)

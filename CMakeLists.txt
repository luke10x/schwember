cmake_minimum_required(VERSION 3.0)
project(MatrixTransform)

# Add the path to the src directory
include_directories(src)

# Set the path to the directory containing GLM headers
set(GLM_INCLUDE_DIR "/opt/homebrew/include")

# Add the source file to the project
add_executable(MatrixTransform src/main.cpp)

# Include the GLM headers
target_include_directories(MatrixTransform PRIVATE ${GLM_INCLUDE_DIR})

# Link any additional libraries if needed (not required in this example)
# target_link_libraries(MatrixTransform PRIVATE <additional_libraries>)

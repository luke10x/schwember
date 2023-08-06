cmake_minimum_required(VERSION 3.0)
project(MatrixTransform)

# Set the path to the directory containing GLM headers
set(GLM_INCLUDE_DIR "/opt/glm")

# Add the source file to the project
add_executable(MatrixTransform /mnt/src/main.cpp)

# Include the GLM headers
target_include_directories(MatrixTransform PRIVATE ${GLM_INCLUDE_DIR})

# Specify the compiler and flags for Emscripten
set(CMAKE_CXX_COMPILER em++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=2 -s USE_WEBGL2=1 -s FULL_ES3=1 -s ALLOW_MEMORY_GROWTH=1 -s GL_UNSAFE_OPTS=0 -s ASSERTIONS=1 -s SAFE_HEAP=1")

# Set the output format to WebAssembly and generate .html file
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# Link any additional libraries if needed (not required in this example)
# target_link_libraries(MatrixTransform PRIVATE <additional_libraries>)

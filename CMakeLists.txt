cmake_minimum_required(VERSION 3.12)
project(schwember)

execute_process(
  COMMAND brew --prefix
  OUTPUT_VARIABLE BREW_PREFIX
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Check if the command was successful
if(NOT BREW_PREFIX)
    message(FATAL_ERROR "Homebrew not found or brew --prefix failed")
endif()

# Add the path to the src directory
include_directories(src)

# Find the GLEW library
find_library(GLEW_LIBRARY GLEW)
find_library(GLFW_LIBRARY glfw)

find_package(Bullet REQUIRED)

# message(FATAL_ERROR "Bullet Physics ${BULLET_LIBRARY}")

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

# Use FetchContent to fetch stb_image
include(FetchContent)
FetchContent_Declare(
    stb_image
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
)
FetchContent_MakeAvailable(stb_image)

# Add the source file to the project
add_executable(schwember src/main.cpp)


# Include headers
include_directories(${BULLET_INCLUDE_DIRS})

target_include_directories(schwember PRIVATE
  ${GLM_INCLUDE_DIR}
  ${cgltf_SOURCE_DIR}
  ${stb_image_SOURCE_DIR}
  ${GLEW_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(schwember PRIVATE
  ${GLEW_LIBRARY}
  ${GLFW_LIBRARY}
  ${BULLET_LIBRARIES}
  "-framework OpenGL"
)

# Set the C++ standard (if needed)
set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 3.12)

# Set the C++ standard (if needed)
set(CMAKE_CXX_STANDARD 11)

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

# Find system-installed libraries
find_library(GLEW_LIBRARY GLEW)
find_library(GLFW_LIBRARY glfw)
find_package(Bullet REQUIRED)

# Set where the ImGui files are stored
set(IMGUI_DIR "external/imgui")
    
# -------------- IMGUI static library creation ------------------------
set(IMGUI_SOURCES
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
# ----------------------------------------------------------------------

# Set the path to the directory containing GLM headers
set(GLM_INCLUDE_DIR "/opt/homebrew/include")


# Add the source file to the project
add_executable(schwember src/main.cpp ${IMGUI_SOURCES})

# Include headers
include_directories(${BULLET_INCLUDE_DIRS})


target_include_directories(schwember PRIVATE
  ${GLM_INCLUDE_DIR}
  external/stb
  external/imgui
  external/imgui/backends
  external/cgltf
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

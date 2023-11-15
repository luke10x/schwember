all: build

CPPFLAGS += -I/opt/glm
CPPFLAGS += -I/opt/cgltf
CPPFLAGS += -I/opt/stb
CPPFLAGS += -I/emsdk/upstream/emscripten/cache/sysroot/include/bullet
CPPFLAGS += -I/mnt/external/imgui
CPPFLAGS += -I/mnt/external/imgui/backends
LDFLAGS += -L/emsdk/upstream/emscripten/cache/sysroot/lib
LDLIBS += -lLinearMath -lBulletDynamics -lBulletCollision

IMGUI_DIR=/mnt/external/imgui
IMGUI_SOURCES += $(IMGUI_DIR)/imgui.cpp
IMGUI_SOURCES += $(IMGUI_DIR)/imgui_demo.cpp
IMGUI_SOURCES += $(IMGUI_DIR)/imgui_draw.cpp
IMGUI_SOURCES += $(IMGUI_DIR)/imgui_tables.cpp
IMGUI_SOURCES += $(IMGUI_DIR)/imgui_widgets.cpp
IMGUI_SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp
IMGUI_SOURCES += $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

EMSCRIPTENFLAGS := -s USE_ZLIB=1 -sNO_DISABLE_EXCEPTION_CATCHING=1 
EMSCRIPTENFLAGS += -s USE_ZLIB=1 -sNO_DISABLE_EXCEPTION_CATCHING=1
EMSCRIPTENFLAGS += -s FULL_ES2=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -O0
EMSCRIPTENFLAGS += -s ALLOW_MEMORY_GROWTH=1 -s GL_UNSAFE_OPTS=0
EMSCRIPTENFLAGS += -s ASSERTIONS=1 -s SAFE_HEAP=1
EMSCRIPTENFLAGS += $(shell ls ../shaders | sed -e \
	's|.*|--preload-file ../shaders/\0@shaders/\0|')
EMSCRIPTENFLAGS += $(shell ls ../models | sed -e \
	's|.*|--preload-file ../models/\0@models/\0|')
EMSCRIPTENFLAGS += $(shell ls ../img | sed -e \
	's|.*|--preload-file ../img/\0@img/\0|')

build:
	mkdir -p ./build
	em++ ../src/main.cpp $(IMGUI_SOURCES) -o ./build/index.html \
		$(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $(EMSCRIPTENFLAGS)

clean:
	rm -rf build

.PHONY: all build clean

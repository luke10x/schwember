all: build

CPPFLAGS += -I/opt/glm
CPPFLAGS += -I/opt/cgltf
CPPFLAGS += -I/opt/stb
CPPFLAGS += -I/emsdk/upstream/emscripten/cache/sysroot/include/bullet
# CPPFLAGS += -I/emsdk/upstream/emscripten/cache/sysroot/include/bullet
# CPPFLAGS += -I/emsdk/upstream/emscripten/cache/sysroot/include/bullet

LDFLAGS += -L/emsdk/upstream/emscripten/cache/sysroot/lib
LDLIBS += -lLinearMath -lBulletDynamics -lBulletCollision

# LDLIBS += -lLinearlMath -lBulletDynamics -lBulletCollision
EMSCRIPTENFLAGS := -s USE_ZLIB=1 -sNO_DISABLE_EXCEPTION_CATCHING=1 
EMSCRIPTENFLAGS += -s USE_ZLIB=1 -sNO_DISABLE_EXCEPTION_CATCHING=1
EMSCRIPTENFLAGS += -s FULL_ES2=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -O0
EMSCRIPTENFLAGS += -s ALLOW_MEMORY_GROWTH=1 -s GL_UNSAFE_OPTS=0
EMSCRIPTENFLAGS += -s ASSERTIONS=1 -s SAFE_HEAP=1
EMSCRIPTENFLAGS += $(shell ls ../src/shaders | sed -e \
	's|.*|--preload-file ../src/shaders/\0@src/shaders/\0|')
EMSCRIPTENFLAGS += $(shell ls ../assets/gltf | sed -e \
	's|.*|--preload-file ../assets/gltf/\0@assets/gltf/\0|')
EMSCRIPTENFLAGS += $(shell ls ../assets/png | sed -e \
	's|.*|--preload-file ../assets/png/\0@assets/png/\0|')

build:
	mkdir -p ./build
	em++ ../src/main.cpp -o ./build/index.html \
		$(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $(EMSCRIPTENFLAGS)

clean:
	rm -rf build

.PHONY: all build clean
all: build

CPPFLAGS += -I/opt/glm
CPPFLAGS += -I/opt/cgltf
CPPFLAGS += -I/emsdk/upstream/emscripten/cache/sysroot/include/bullet
# CPPFLAGS += -I/emsdk/upstream/emscripten/cache/sysroot/include/bullet
# CPPFLAGS += -I/emsdk/upstream/emscripten/cache/sysroot/include/bullet
LDFLAGS += -L/emsdk/upstream/emscripten/cache/sysroot/lib
# LDLIBS += -lLinearlMath -lBulletDynamics -lBulletCollision

build:

	mkdir -p ./build
	em++ \
		../src/main.cpp \
		-o ./build/index.html \
		$(CPPFLAGS) $(LDFLAGS) $(LDLIBS) \
		-s FULL_ES2=1 -s USE_GLFW=3 -s USE_WEBGL2=1 -O0 \
		-s ALLOW_MEMORY_GROWTH=1 -s GL_UNSAFE_OPTS=0 \
		-s ASSERTIONS=1 -s SAFE_HEAP=1 \
		--preload-file ../assets/gltf/cubeman.glb@assets/gltf/cubeman.glb

clean:
	rm -rf build

.PHONY: all build clean
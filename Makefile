all: build

build:
	mkdir -p build
	cd build && cmake ..
	make -C build

clean:
	rm -rf build

clean-assets:
	rm -rf assets/gltf

.PHONY: all build clean clean-assets

assets/gltf:
	mkdir -p assets/gltf
	blender -b resources/cubeman.blend -o assets/obj/cubeman.gltf --python-expr \
		"import bpy; bpy.ops.export_scene.gltf(filepath='assets/gltf/cubeman.gltf')"
all: build

build:
	mkdir -p build
	cd build && cmake .. --debug-output
	# && cmake ..
	make -C build VERBOSE=1

clean:
	rm -rf build

clean-assets:
	rm -rf assets/gltf

run:
	@echo "Press any key..."
	@dd bs=1 count=1 2>/dev/null
	./build/MatrixTransform

.PHONY: all clean clean-assets run

assets/gltf:
	mkdir -p assets/gltf
	blender -b resources/cubeman.blend -o assets/obj/cubeman.gltf --python-expr \
		"import bpy; bpy.ops.export_scene.gltf(filepath='assets/gltf/cubeman.gltf')"
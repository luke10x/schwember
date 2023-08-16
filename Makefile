all: build

build/schwember:
	mkdir -p build
	cd build && cmake .. --debug-output
	make -C build VERBOSE=1

clean:
	rm -fr build/CMakeFiles/
	rm -f  build/CMakeCache.txt
	rm -f  build/Makefile
	rm -f  build/cmake_install.cmake 
	rm -f  build/schwember

remove:
	rm -rf build

clean-assets:
	rm -rf assets/gltf

run: build/schwember
	@echo "Press any key..."
	@dd bs=1 count=1 2>/dev/null
	./build/schwember

.PHONY: all clean clean-assets run

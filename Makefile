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

dbg: build/schwember
	@echo "\nLLDB Cheatsheet:"
	@echo "\tbreak -n pc_start # Sets breakpoint into function"
	@echo "\tbreak -n 88       # Sets breakpoint on line number"
	@echo "\tclear             # Clears all breakpoints"
	@echo "\trun               # runs program"
	@echo "\tlist              # prints code"
	@echo "\tlist -            # lists back"
	@echo "\tframe info        # prints where I am"
	@echo "\tnext              # next line"
	@echo "\tcont              # continues execution"
	@echo "\tquit              # quits debugging"
	@echo "Press any key to debug..."
	@dd bs=1 count=1 2>/dev/null
	lldb ./build/schwember

.PHONY: all clean clean-assets run

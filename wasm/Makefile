CMAKE := emcmake cmake
CMAKE_BUILD := emmake make

all: build

build:
	mkdir -p build
	cd build && $(CMAKE) ..
	$(CMAKE_BUILD) -C build

clean:
	rm -rf build

.PHONY: all build clean
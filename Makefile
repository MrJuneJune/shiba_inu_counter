# Default build type is Debug unless specified
BUILD_TYPE ?= Debug

all: compile

compile:
	mkdir -p build && cd build && cmake ../ -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) && make -j

release:
	$(MAKE) BUILD_TYPE=Release

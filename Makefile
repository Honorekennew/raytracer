# Shortcuts for the commands in README.md. Run them inside the dev container.
# This file only calls the CMake presets: the real build is described in CMakeLists.txt.
#
#   make build           compile
#   make run             compile, then start the program
#   make run ARGS=Ada    same, passing arguments to the program
#   make test            compile, then run the tests
#   make clean           delete the build/ folder
#   make build PRESET=release    use another preset (debug, release, sanitize)

PRESET ?= debug
ARGS ?=

.PHONY: help build run test clean

help:
	@echo "make build   compile the project"
	@echo "make run     compile and start the program (ARGS=Ada to pass arguments)"
	@echo "make test    compile and run the tests"
	@echo "make clean   delete the build/ folder"
	@echo "Add PRESET=release or PRESET=sanitize to use another preset (default: debug)."

# Configure only when the preset has never been configured.
# CMake re-configures by itself afterwards when a CMakeLists.txt changes.
build/$(PRESET)/build.ninja:
	cmake --preset $(PRESET)

build: build/$(PRESET)/build.ninja
	cmake --build --preset $(PRESET)

run: build
	./build/$(PRESET)/src/raytracer $(ARGS)

test: build
	ctest --preset $(PRESET)

clean:
	rm -rf build

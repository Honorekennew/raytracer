include(FetchContent)

# LodePNG encodes and decodes PNG files (see ADR 6). It has no tags, so it is pinned to a commit
# (version 20261102). It is downloaded at the first configuration, like Catch2.
# Upstream has no CMakeLists.txt, so "MakeAvailable" only downloads it, and we build it below.
FetchContent_Declare(
  lodepng
  GIT_REPOSITORY https://github.com/lvandeve/lodepng.git
  GIT_TAG        7b2893e35891c0c85bee5595caafaf3c2044b409)
FetchContent_MakeAvailable(lodepng)

# A library of its own, one source file. It does not link raytracer_options: our warning flags
# (and -Werror) are for our code, not for a third-party file. SYSTEM keeps warnings away from the
# code that includes <lodepng.h> too.
add_library(lodepng STATIC ${lodepng_SOURCE_DIR}/lodepng.cpp)
add_library(lodepng::lodepng ALIAS lodepng)
target_include_directories(lodepng SYSTEM PUBLIC ${lodepng_SOURCE_DIR})

# The library holds all the logic so that both the executable and the tests can use it.
# Sources are grouped by concept: one folder per concept (colour/, geometry/, io/, math/, render/).
add_library(raytracer_core
  colour/colour.cpp
  geometry/ray.cpp
  geometry/sphere.cpp
  io/png_saver.cpp
  math/vector.cpp
  render/camera.cpp
  render/image.cpp)
add_library(raytracer::core ALIAS raytracer_core)

target_include_directories(raytracer_core PUBLIC
  ${PROJECT_SOURCE_DIR}/include)
# PRIVATE: only png_saver.cpp uses LodePNG, no header of ours includes <lodepng.h>.
target_link_libraries(raytracer_core PRIVATE lodepng::lodepng raytracer_options)

# The command-line program: thin, it only wires the library to the outside world.
add_executable(raytracer main.cpp)
target_link_libraries(raytracer PRIVATE raytracer::core raytracer_options)

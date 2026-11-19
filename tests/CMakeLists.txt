include(FetchContent)

# Catch2 is used from the system if a compatible version is installed,
# and downloaded (pinned version) otherwise.
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
  GIT_SHALLOW    TRUE
  FIND_PACKAGE_ARGS 3.7 CONFIG)
FetchContent_MakeAvailable(Catch2)

# Provides catch_discover_tests(), which registers every TEST_CASE with CTest.
if(Catch2_SOURCE_DIR)
  list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
endif()
include(Catch)

# Add new test files to this list. Tests are grouped by concept, like the sources in src/.
add_executable(raytracer_tests
  test_helpers.cpp
  colour/colour_test.cpp
  geometry/hit_test.cpp
  geometry/plane_test.cpp
  geometry/ray_test.cpp
  geometry/sphere_test.cpp
  io/png_saver_test.cpp
  light/light_test.cpp
  material/flat_shader_test.cpp
  material/lambert_shader_test.cpp
  math/vector_test.cpp
  render/camera_test.cpp
  render/image_test.cpp
  sanity_test.cpp)
# Lets every test file write #include "test_helpers.hpp".
target_include_directories(raytracer_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# lodepng: the PngSaver tests read the PNG file back with LodePNG's decoder.
target_link_libraries(raytracer_tests PRIVATE
  raytracer::core
  lodepng::lodepng
  raytracer_options
  Catch2::Catch2WithMain)

catch_discover_tests(raytracer_tests)

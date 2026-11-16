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

# Add new test files to this list.
add_executable(raytracer_tests
  greeting_test.cpp)
target_link_libraries(raytracer_tests PRIVATE
  raytracer::core
  raytracer_options
  Catch2::Catch2WithMain)

catch_discover_tests(raytracer_tests)

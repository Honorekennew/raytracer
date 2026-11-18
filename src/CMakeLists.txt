# The library holds all the logic so that both the executable and the tests can use it.
# Sources are grouped by concept: one folder per concept (math/, demo/, ...).
add_library(raytracer_core
  demo/greeting.cpp
  math/ray.cpp
  math/sphere.cpp
  math/vector.cpp)
add_library(raytracer::core ALIAS raytracer_core)

target_include_directories(raytracer_core PUBLIC
  ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(raytracer_core PRIVATE raytracer_options)

# The command-line program: thin, it only wires the library to the outside world.
add_executable(raytracer main.cpp)
target_link_libraries(raytracer PRIVATE raytracer::core raytracer_options)

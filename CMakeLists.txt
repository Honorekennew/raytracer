cmake_minimum_required(VERSION 3.24)

project(raytracer
  VERSION 0.1.0
  DESCRIPTION "Ray tracer for a film studio"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(RAYTRACER_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(RAYTRACER_SANITIZE "Build with AddressSanitizer and UndefinedBehaviorSanitizer (GCC/Clang)" OFF)

# One place for warning and sanitizer flags, shared by every target that links it.
add_library(raytracer_options INTERFACE)
if(MSVC)
  target_compile_options(raytracer_options INTERFACE /W4 /permissive-)
  if(RAYTRACER_WARNINGS_AS_ERRORS)
    target_compile_options(raytracer_options INTERFACE /WX)
  endif()
else()
  target_compile_options(raytracer_options INTERFACE -Wall -Wextra -Wpedantic)
  if(RAYTRACER_WARNINGS_AS_ERRORS)
    target_compile_options(raytracer_options INTERFACE -Werror)
  endif()
  if(RAYTRACER_SANITIZE)
    target_compile_options(raytracer_options INTERFACE
      -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(raytracer_options INTERFACE -fsanitize=address,undefined)
  endif()
endif()

add_subdirectory(src)

include(CTest) # defines BUILD_TESTING (ON by default)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

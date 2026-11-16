#include <catch2/catch_test_macros.hpp>

#include "raytracer/greeting.hpp"

TEST_CASE("greeting addresses the given name", "[greeting]") {
  REQUIRE(raytracer::greeting("World") == "Hello, World!");
  REQUIRE(raytracer::greeting("Ada") == "Hello, Ada!");
}

TEST_CASE("greeting without a name is a plain hello", "[greeting]") {
  REQUIRE(raytracer::greeting("") == "Hello!");
}

#include <catch2/catch_test_macros.hpp>

#include "raytracer/colour/colour.hpp"

// Compared with plain numbers, not requireColoursEqual: this test checks r(), g() and b()
// themselves. Nothing is calculated, and 0.5 is exact in binary.
TEST_CASE("Colour getters return r, g, b", "[colour]") {
  raytracer::Colour colour(1, 0.5, 0);

  REQUIRE(colour.r() == 1);
  REQUIRE(colour.g() == 0.5);
  REQUIRE(colour.b() == 0);
}

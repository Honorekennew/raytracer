#include <catch2/catch_test_macros.hpp>

#include "raytracer/math/vector.hpp"

// The numbers used here are whole numbers, so the results are exact and we can
// compare them with ==. (Later, results of more complex calculations will need
// Catch::Approx instead.)

TEST_CASE("adding two vectors adds each component", "[vector]") {
  raytracer::Vector a(1, 2, 3);
  raytracer::Vector b(4, 5, 6);

  raytracer::Vector result = a + b;

  REQUIRE(result.x() == 5.0);
  REQUIRE(result.y() == 7.0);
  REQUIRE(result.z() == 9.0);
}

TEST_CASE("subtracting a vector subtracts each component", "[vector]") {
  raytracer::Vector a(-1, 4, 0);
  raytracer::Vector b(-5, 2, 1);

  raytracer::Vector result = a - b;

  REQUIRE(result.x() == 4.0);
  REQUIRE(result.y() == 2.0);
  REQUIRE(result.z() == -1.0);
}

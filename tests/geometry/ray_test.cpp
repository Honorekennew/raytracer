#include <catch2/catch_test_macros.hpp>

#include "raytracer/geometry/ray.hpp"
#include "raytracer/math/vector.hpp"
#include "test_helpers.hpp"

using raytracer::Ray;
using raytracer::Vector;

TEST_CASE("Ray getters return origin and direction", "[ray]") {
  Ray ray(Vector(1, 2, 3), Vector(3, 4, 0));

  requireVectorsEqual(ray.origin(), Vector(1, 2, 3));
  requireVectorsEqual(ray.direction(), Vector(3, 4, 0));
}

// The point of the ray at "t" is origin + t * direction.
//   t = 2: (1, 2, 3) + 2 * (3, 4, 0) = (7, 10, 3)
TEST_CASE("Ray at(t) gives the point origin + t * direction", "[ray]") {
  Ray ray(Vector(1, 2, 3), Vector(3, 4, 0));

  requireVectorsEqual(ray.at(2), Vector(7, 10, 3));
}

// t = 0 is the origin itself, and a negative t is behind it.
TEST_CASE("Ray at(0) is the origin and at(-1) is behind it", "[ray]") {
  Ray ray(Vector(1, 2, 3), Vector(3, 4, 0));

  requireVectorsEqual(ray.at(0), Vector(1, 2, 3));
  requireVectorsEqual(ray.at(-1), Vector(-2, -2, 3));
}

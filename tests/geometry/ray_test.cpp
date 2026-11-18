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

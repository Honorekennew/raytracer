#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "raytracer/geometry/ray.hpp"
#include "raytracer/math/vector.hpp"
#include "test_helpers.hpp"

using Catch::Matchers::WithinAbs;
using raytracer::Ray;
using raytracer::Vector;

TEST_CASE("Vector add", "[vector]") {
  Vector a(1, 2, 3);
  Vector b(4, 5, 6);

  requireVectorsEqual(a + b, Vector(5, 7, 9));
}

TEST_CASE("Vector subtract", "[vector]") {
  Vector a(-1, 4, 0);
  Vector b(-5, 2, 1);

  requireVectorsEqual(a - b, Vector(4, 2, -1));
}

TEST_CASE("Vector scalar multiply (scalar * vector)", "[vector]") {
  Vector v(1, -2, 3);

  requireVectorsEqual(2.0 * v, Vector(2, -4, 6));
}

TEST_CASE("Vector scalar multiply (vector * scalar)", "[vector]") {
  Vector v(1, -2, 3);

  requireVectorsEqual(v * 2.0, Vector(2, -4, 6));
}

// 1 * 4 + 2 * -5 + 3 * 6 = 4 - 10 + 18 = 12
TEST_CASE("Vector dot product", "[vector]") {
  Vector a(1, 2, 3);
  Vector b(4, -5, 6);

  REQUIRE_THAT(a.dot(b), WithinAbs(12.0, tolerance));
}

// The 3-4-5 triangle, and 1-2-2 (1 + 4 + 4 = 9).
TEST_CASE("Vector length", "[vector]") {
  REQUIRE_THAT(Vector(3, 4, 0).length(), WithinAbs(5.0, tolerance));
  REQUIRE_THAT(Vector(1, 2, 2).length(), WithinAbs(3.0, tolerance));
}

// (0, 3, 4) has length 5, so its normalised vector is (0, 3/5, 4/5) = (0, 0.6, 0.8).
TEST_CASE("Vector normalised", "[vector]") {
  Vector normalised = Vector(0, 3, 4).normalised();

  requireVectorsEqual(normalised, Vector(0, 0.6, 0.8));
  REQUIRE_THAT(normalised.length(), WithinAbs(1.0, tolerance));
}

// A point of a ray is O + t * D. With O = (0, 0, -1) and D = (0, 0, 1):
// t = 5 gives (0, 0, 4), the front of our sphere, and t = 15 gives (0, 0, 14), the back.
TEST_CASE("Ray equation O + t * D", "[ray]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));

  requireVectorsEqual(ray.origin() + 5.0 * ray.direction(), Vector(0, 0, 4));
  requireVectorsEqual(ray.origin() + ray.direction() * 15.0, Vector(0, 0, 14));
}

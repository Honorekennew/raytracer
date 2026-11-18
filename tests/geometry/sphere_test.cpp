#include <catch2/catch_test_macros.hpp>

#include "raytracer/geometry/ray.hpp"
#include "raytracer/geometry/sphere.hpp"
#include "raytracer/math/vector.hpp"
#include "test_helpers.hpp"

using raytracer::Ray;
using raytracer::Sphere;
using raytracer::Vector;

// All the rays are the ray of our camera: origin (0, 0, -1), direction (0, 0, 1).
// The spheres have radius 5. The numbers come from the maths:
//   centre (0, 0, 9):   L = (0, 0, -10),   b = -20, c = 75,  discriminant = 100,  t = 5 or 15
//   centre (10, 0, 9):  L = (-10, 0, -10), b = -20, c = 175, discriminant = -300, no t
//   centre (0, 0, -20): L = (0, 0, 19),    b = 38,  c = 336, discriminant = 100,  t = -14 or -24
//   centre (5, 0, 9):   L = (-5, 0, -10),  b = -20, c = 100, discriminant = 0,    t = 10 (twice)

TEST_CASE("Sphere at (0,0,9) radius 5: getters and ray hit", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere(Vector(0, 0, 9), 5);

  requireVectorsEqual(sphere.center(), Vector(0, 0, 9));
  REQUIRE(sphere.radius() == 5);
  REQUIRE(sphere.hit(ray));
}

TEST_CASE("Sphere at (10,0,9) radius 5: getters and ray miss", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere(Vector(10, 0, 9), 5);

  requireVectorsEqual(sphere.center(), Vector(10, 0, 9));
  REQUIRE(sphere.radius() == 5);
  REQUIRE_FALSE(sphere.hit(ray));
}

TEST_CASE("Sphere at (0,0,-20) radius 5: getters and ray miss (behind ray)", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere(Vector(0, 0, -20), 5);

  requireVectorsEqual(sphere.center(), Vector(0, 0, -20));
  REQUIRE(sphere.radius() == 5);
  REQUIRE_FALSE(sphere.hit(ray));
}

// The ray only touches the sphere (discriminant = 0). We decided that touching counts as a hit.
TEST_CASE("Sphere at (5,0,9) radius 5: getters and ray hit (edge)", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere(Vector(5, 0, 9), 5);

  requireVectorsEqual(sphere.center(), Vector(5, 0, 9));
  REQUIRE(sphere.radius() == 5);
  REQUIRE(sphere.hit(ray));
}

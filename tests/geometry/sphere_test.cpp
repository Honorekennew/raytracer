#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <memory>
#include <stdexcept>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/geometry/sphere.hpp"
#include "raytracer/light/light.hpp"
#include "raytracer/material/flat_shader.hpp"
#include "raytracer/math/vector.hpp"
#include "test_helpers.hpp"

using Catch::Matchers::WithinAbs;
using raytracer::Colour;
using raytracer::FlatShader;
using raytracer::Hit;
using raytracer::Light;
using raytracer::Ray;
using raytracer::Sphere;
using raytracer::Vector;

namespace {

// A sphere that is one flat colour.
Sphere flatSphere(const Vector& center, double radius, const Colour& colour) {
  return Sphere(center, radius, std::make_shared<FlatShader>(colour));
}

}  // namespace

// All the rays are the ray of our camera: origin (0, 0, -1), direction (0, 0, 1).
// The spheres have radius 5. The numbers come from the maths:
//   centre (0, 0, 9):   L = (0, 0, -10),   b = -20, c = 75,  discriminant = 100,  t = 5 or 15
//   centre (10, 0, 9):  L = (-10, 0, -10), b = -20, c = 175, discriminant = -300, no t
//   centre (0, 0, -20): L = (0, 0, 19),    b = 38,  c = 336, discriminant = 100,  t = -14 or -24
//   centre (5, 0, 9):   L = (-5, 0, -10),  b = -20, c = 100, discriminant = 0,    t = 10 (twice)

TEST_CASE("Sphere at (0,0,9) radius 5: getters and ray hit", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere = flatSphere(Vector(0, 0, 9), 5, Colour(1, 1, 1));

  requireVectorsEqual(sphere.center(), Vector(0, 0, 9));
  REQUIRE(sphere.radius() == 5);
  REQUIRE(sphere.hit(ray));
}

TEST_CASE("Sphere at (10,0,9) radius 5: getters and ray miss", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere = flatSphere(Vector(10, 0, 9), 5, Colour(1, 1, 1));

  requireVectorsEqual(sphere.center(), Vector(10, 0, 9));
  REQUIRE(sphere.radius() == 5);
  REQUIRE_FALSE(sphere.hit(ray));
}

TEST_CASE("Sphere at (0,0,-20) radius 5: getters and ray miss (behind ray)", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere = flatSphere(Vector(0, 0, -20), 5, Colour(1, 1, 1));

  requireVectorsEqual(sphere.center(), Vector(0, 0, -20));
  REQUIRE(sphere.radius() == 5);
  REQUIRE_FALSE(sphere.hit(ray));
}

// The ray only touches the sphere (discriminant = 0). We decided that touching counts as a hit.
TEST_CASE("Sphere at (5,0,9) radius 5: getters and ray hit (edge)", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere = flatSphere(Vector(5, 0, 9), 5, Colour(1, 1, 1));

  requireVectorsEqual(sphere.center(), Vector(5, 0, 9));
  REQUIRE(sphere.radius() == 5);
  REQUIRE(sphere.hit(ray));
}

// The sphere keeps the material it was given, and the material decides the colour.
TEST_CASE("Sphere keeps the material it was created with", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Light light(Vector(0, 0, -5), Colour(1, 1, 1));
  Sphere sphere = flatSphere(Vector(0, 0, 9), 5, Colour(1, 1, 0));
  auto hit = sphere.hit(ray);
  REQUIRE(hit.has_value());

  requireColoursEqual(sphere.material().shade(ray, *hit, light), Colour(1, 1, 0));
}

// Several spheres can share one material.
TEST_CASE("Two spheres can share the same material", "[sphere]") {
  auto yellow = std::make_shared<FlatShader>(Colour(1, 1, 0));
  Sphere first(Vector(0, 0, 9), 5, yellow);
  Sphere second(Vector(10, 0, 9), 1, yellow);
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Light light(Vector(0, 0, -5), Colour(1, 1, 1));
  Hit hit(5, Vector(0, 0, 4), Vector(0, 0, -1), *yellow);

  requireColoursEqual(first.material().shade(ray, hit, light), Colour(1, 1, 0));
  requireColoursEqual(second.material().shade(ray, hit, light), Colour(1, 1, 0));
}

// hit() also says WHERE the ray hits and how the surface is turned there. The ray (0, 0, -1) +
// t * (0, 0, 1) enters the sphere of centre (0, 0, 9) and radius 5 at t = 5, so at (0, 0, 4).
// The normal goes from the centre to that point: (0, 0, 4) - (0, 0, 9) = (0, 0, -5), and it is
// made 1 long: (0, 0, -1). It points towards the camera.
TEST_CASE("Sphere hit gives the point and the normal of the first hit", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere = flatSphere(Vector(0, 0, 9), 5, Colour(1, 1, 1));

  auto hit = sphere.hit(ray);

  REQUIRE(hit.has_value());
  REQUIRE_THAT(hit->t(), WithinAbs(5, tolerance));
  requireVectorsEqual(hit->point(), Vector(0, 0, 4));
  requireVectorsEqual(hit->normal(), Vector(0, 0, -1));
}

// The hit carries the material of the sphere that was hit: the very same object.
TEST_CASE("Sphere hit carries the material of the sphere", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere = flatSphere(Vector(0, 0, 9), 5, Colour(1, 0, 0));

  auto hit = sphere.hit(ray);

  REQUIRE(hit.has_value());
  REQUIRE(&hit->material() == &sphere.material());
}

// The direction of a ray does not have to be 1 long: (0, 0, 2) reaches the same point at t = 2.5.
TEST_CASE("Sphere hit does not depend on the length of the ray direction", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 2));
  Sphere sphere = flatSphere(Vector(0, 0, 9), 5, Colour(1, 1, 1));

  auto hit = sphere.hit(ray);

  REQUIRE(hit.has_value());
  REQUIRE_THAT(hit->t(), WithinAbs(2.5, tolerance));
  requireVectorsEqual(hit->point(), Vector(0, 0, 4));
  requireVectorsEqual(hit->normal(), Vector(0, 0, -1));
}

// A normal that is not along an axis. The ray (3, 4, -20) + t * (0, 0, 1) meets the sphere of
// centre (0, 0, 0) and radius 13 where 3^2 + 4^2 + z^2 = 13^2, so z = -12 (first) or 12. The
// first hit is (3, 4, -12) at t = 8, and its normal is (3, 4, -12) / 13.
TEST_CASE("Sphere hit gives a normal of length 1 that points out of the sphere", "[sphere]") {
  Ray ray(Vector(3, 4, -20), Vector(0, 0, 1));
  Sphere sphere = flatSphere(Vector(0, 0, 0), 13, Colour(1, 1, 1));

  auto hit = sphere.hit(ray);

  REQUIRE(hit.has_value());
  requireVectorsEqual(hit->point(), Vector(3, 4, -12));
  requireVectorsEqual(hit->normal(), Vector(3.0 / 13, 4.0 / 13, -12.0 / 13));
}

// The ray starts inside the sphere: the point behind it (t < 0) does not count, so the hit is
// the exit point, (0, 0, 14).
TEST_CASE("Sphere hit from inside gives the point where the ray leaves", "[sphere]") {
  Ray ray(Vector(0, 0, 9), Vector(0, 0, 1));
  Sphere sphere = flatSphere(Vector(0, 0, 9), 5, Colour(1, 1, 1));

  auto hit = sphere.hit(ray);

  REQUIRE(hit.has_value());
  requireVectorsEqual(hit->point(), Vector(0, 0, 14));
  requireVectorsEqual(hit->normal(), Vector(0, 0, 1));
}

TEST_CASE("Sphere hit gives nothing when the ray misses", "[sphere]") {
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Sphere sphere = flatSphere(Vector(10, 0, 9), 5, Colour(1, 1, 1));

  REQUIRE_FALSE(sphere.hit(ray).has_value());
}

// A sphere without a material could not be shaded: we refuse it when it is created.
TEST_CASE("Sphere without a material is refused", "[sphere]") {
  REQUIRE_THROWS_AS(Sphere(Vector(0, 0, 9), 5, nullptr), std::invalid_argument);
}

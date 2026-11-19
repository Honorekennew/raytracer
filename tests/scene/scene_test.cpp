#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <memory>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/plane.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/geometry/sphere.hpp"
#include "raytracer/material/flat_shader.hpp"
#include "raytracer/math/vector.hpp"
#include "raytracer/scene/scene.hpp"
#include "test_helpers.hpp"

using Catch::Matchers::WithinAbs;
using raytracer::Colour;
using raytracer::FlatShader;
using raytracer::Hit;
using raytracer::Plane;
using raytracer::Ray;
using raytracer::Scene;
using raytracer::Sphere;
using raytracer::Vector;

namespace {

const Colour red(1, 0, 0);
const Colour blue(0, 0, 1);

// The ray of our camera: from (0, 0, -1) straight ahead, along +Z.
Ray cameraRay() { return Ray(Vector(0, 0, -1), Vector(0, 0, 1)); }

// A red sphere with the centre (0, 0, 9) and the radius 5. The ray above hits it at t = 5.
std::unique_ptr<Sphere> redSphere() {
  return std::make_unique<Sphere>(Vector(0, 0, 9), 5, std::make_shared<FlatShader>(red));
}

// A blue plane through (0, 0, "z"), whose normal (0, 0, -1) faces the camera.
std::unique_ptr<Plane> bluePlaneAt(double z) {
  return std::make_unique<Plane>(Vector(0, 0, z), Vector(0, 0, -1),
                                 std::make_shared<FlatShader>(blue));
}

// The colour that the material of the hit gives. The material is a FlatShader, so the ray and the
// sun make no difference: only its colour counts.
Colour colourOf(const Hit& hit) {
  return hit.material().shade(cameraRay(), hit, Vector(0, 0, -5));
}

}  // namespace

// The scene gives the direction towards the sun with length 1: (0, 3, -4) is 5 long, so it
// becomes (0, 3, -4) / 5 = (0, 0.6, -0.8).
TEST_CASE("Scene keeps the direction towards the sun as a unit vector", "[scene]") {
  Scene scene(Vector(0, 3, -4));

  requireVectorsEqual(scene.toLight(), Vector(0, 0.6, -0.8));
}

// (0, 0, 0) has no direction: it can't be made 1 long, so the scene keeps it. It must not become
// "not a number".
TEST_CASE("Scene keeps a sun with no direction as it is", "[scene]") {
  Scene scene(Vector(0, 0, 0));

  requireVectorsEqual(scene.toLight(), Vector(0, 0, 0));
}

// The ray (0, 0, -1) + t * (0, 0, 1) meets two shapes:
//   - the sphere, at t = 5 (it enters the sphere at (0, 0, 4));
//   - the plane through (0, 0, 2) with the normal (0, 0, -1):
//     n . (Q - O) = (0, 0, -1) . (0, 0, 3) = -3     n . D = -1     t = -3 / -1 = 3.
// The plane is nearer (3 < 5): its hit is the one the scene gives, and its colour is blue.
TEST_CASE("Scene: the nearest hit is the plane in front of the sphere, at t = 3", "[scene]") {
  Scene scene(Vector(0, 1, 0));
  scene.add(redSphere());
  scene.add(bluePlaneAt(2));

  auto hit = scene.nearestHit(cameraRay());

  REQUIRE(hit.has_value());
  REQUIRE_THAT(hit->t(), WithinAbs(3, tolerance));
  requireColoursEqual(colourOf(*hit), blue);
}

// The plane through (0, 0, 20): n . (Q - O) = (0, 0, -1) . (0, 0, 21) = -21, n . D = -1, t = 21.
// The sphere is nearer (5 < 21): its hit is the one the scene gives, and its colour is red.
// The sphere was added first here, and the plane first in the test above: the answer does not
// depend on the order.
TEST_CASE("Scene: the nearest hit is the sphere in front of the plane, at t = 5", "[scene]") {
  Scene scene(Vector(0, 1, 0));
  scene.add(redSphere());
  scene.add(bluePlaneAt(20));

  auto hit = scene.nearestHit(cameraRay());

  REQUIRE(hit.has_value());
  REQUIRE_THAT(hit->t(), WithinAbs(5, tolerance));
  requireColoursEqual(colourOf(*hit), red);
}

// The sphere of centre (6, 0, 9) and radius 5 is 6 away from the ray, which runs along the z
// axis: more than the radius, the ray misses it. No shape is hit, so there is no hit.
TEST_CASE("Scene: a ray that hits no shape gives nothing", "[scene]") {
  Scene scene(Vector(0, 1, 0));
  scene.add(std::make_unique<Sphere>(Vector(6, 0, 9), 5, std::make_shared<FlatShader>(red)));

  REQUIRE_FALSE(scene.nearestHit(cameraRay()).has_value());
}

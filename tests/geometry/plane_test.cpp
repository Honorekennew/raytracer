#include <catch2/catch_test_macros.hpp>

#include <memory>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/plane.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/light/light.hpp"
#include "raytracer/material/flat_shader.hpp"
#include "raytracer/math/vector.hpp"
#include "test_helpers.hpp"

using raytracer::Colour;
using raytracer::FlatShader;
using raytracer::Light;
using raytracer::Plane;
using raytracer::Ray;
using raytracer::Vector;

namespace {

// A plane with a green material.
Plane greenPlane(const Vector& point, const Vector& normal) {
  return Plane(point, normal, std::make_shared<FlatShader>(Colour(0, 1, 0)));
}

// The floor of the client: it goes through (0, 0, 0) and its normal is (0, 1, 0), so it is the
// plane y = 0.
Plane floorPlane() { return greenPlane(Vector(0, 0, 0), Vector(0, 1, 0)); }

}  // namespace

// The ray (0, 8, 0) + t * (3, -4, 0) comes down towards the floor.
//   n . (Q - O) = (0, 1, 0) . (0, -8, 0) = -8      n . D = (0, 1, 0) . (3, -4, 0) = -4
//   t = -8 / -4 = 2, and the point is (0, 8, 0) + 2 * (3, -4, 0) = (6, 0, 0).
TEST_CASE("Plane: the ray (0,8,0) + t * (3,-4,0) hits the floor at t = 2", "[plane]") {
  Plane plane = floorPlane();
  Ray ray(Vector(0, 8, 0), Vector(3, -4, 0));

  auto hit = plane.hit(ray);

  REQUIRE(hit.has_value());
  requireVectorsEqual(hit->point(), Vector(6, 0, 0));
  requireVectorsEqual(hit->normal(), Vector(0, 1, 0));

  // The material of the plane is green.
  Light light(Vector(0, 10, 0), Colour(1, 1, 1));
  requireColoursEqual(plane.material().shade(ray, *hit, light), Colour(0, 1, 0));
}

// The line 3x + 4y = 24 (in 3D, the plane that contains it and the z axis). It goes through
// (8, 0, 0), and (3, 4, 0) is perpendicular to it. That normal is 5 long: the plane keeps it
// normalised, (3, 4, 0) / 5 = (0.6, 0.8, 0), and the hit has that normal.
//   O = (-8, 0, 0), D = (4, 3, 0)    Q - O = (16, 0, 0)
//   n . (Q - O) = 48      n . D = 12 + 12 = 24      t = 48 / 24 = 2
//   the point is (-8, 0, 0) + 2 * (4, 3, 0) = (0, 6, 0), and 3 * 0 + 4 * 6 = 24: it is on the line.
TEST_CASE("Plane: the ray (-8,0,0) + t * (4,3,0) hits the line 3x + 4y = 24 at t = 2", "[plane]") {
  Plane plane = greenPlane(Vector(8, 0, 0), Vector(3, 4, 0));
  Ray ray(Vector(-8, 0, 0), Vector(4, 3, 0));

  auto hit = plane.hit(ray);

  REQUIRE(hit.has_value());
  requireVectorsEqual(hit->point(), Vector(0, 6, 0));
  requireVectorsEqual(hit->normal(), Vector(0.6, 0.8, 0));
}

// The ray (0, 8, 0) + t * (5, 0, 0) stays at the height y = 8, so it never reaches the floor.
//   n . D = (0, 1, 0) . (5, 0, 0) = 0: the ray is parallel to the plane, there is no hit.
TEST_CASE("Plane: a ray parallel to the floor hits nothing", "[plane]") {
  Plane plane = floorPlane();
  Ray ray(Vector(0, 8, 0), Vector(5, 0, 0));

  REQUIRE_FALSE(plane.hit(ray).has_value());
}

// The ray (0, 8, 0) + t * (3, 4, 0) goes up, away from the floor.
//   n . (Q - O) = -8      n . D = (0, 1, 0) . (3, 4, 0) = 4      t = -8 / 4 = -2
// t is not above 0: the floor is behind the ray, there is no hit.
TEST_CASE("Plane: a ray that goes away from the floor hits nothing", "[plane]") {
  Plane plane = floorPlane();
  Ray ray(Vector(0, 8, 0), Vector(3, 4, 0));

  REQUIRE_FALSE(plane.hit(ray).has_value());
}

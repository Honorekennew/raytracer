#include <catch2/catch_test_macros.hpp>

#include "raytracer/geometry/ray.hpp"
#include "raytracer/math/vector.hpp"
#include "raytracer/render/camera.hpp"
#include "test_helpers.hpp"

using raytracer::Camera;
using raytracer::Ray;
using raytracer::Vector;

TEST_CASE("Ray through center has camera position as origin", "[camera]") {
  Camera camera(Vector(1, 2, 3));

  Ray ray = camera.rayThroughCenter();

  requireVectorsEqual(camera.position(), Vector(1, 2, 3));
  requireVectorsEqual(ray.origin(), Vector(1, 2, 3));
}

// The camera faces +Z, wherever it is.
TEST_CASE("Ray through center always points in +Z direction", "[camera]") {
  Camera first(Vector(0, 0, -1));
  Camera second(Vector(5, -3, 2));

  requireVectorsEqual(first.rayThroughCenter().direction(), Vector(0, 0, 1));
  requireVectorsEqual(second.rayThroughCenter().direction(), Vector(0, 0, 1));
}

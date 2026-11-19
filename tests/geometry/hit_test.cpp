#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/ray.hpp"
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
using raytracer::Vector;

TEST_CASE("Hit getters return t, point, normal and material", "[hit]") {
  FlatShader material(Colour(0, 1, 0));
  Hit hit(2.5, Vector(1, 2, 3), Vector(0, 0, 1), material);

  REQUIRE_THAT(hit.t(), WithinAbs(2.5, tolerance));
  requireVectorsEqual(hit.point(), Vector(1, 2, 3));
  requireVectorsEqual(hit.normal(), Vector(0, 0, 1));

  // It is the material we gave, not a copy: shading with it gives its green.
  Ray ray(Vector(0, 0, -1), Vector(0, 0, 1));
  Light light(Vector(0, 0, -5), Colour(1, 1, 1));
  requireColoursEqual(hit.material().shade(ray, hit, light), Colour(0, 1, 0));
}

// A Hit can be copied and put in a std::optional, which is what the shapes return.
TEST_CASE("Hit can be copied and keeps its material", "[hit]") {
  FlatShader material(Colour(0, 1, 0));
  Hit hit(2.5, Vector(1, 2, 3), Vector(0, 0, 1), material);

  Hit copy = hit;

  REQUIRE_THAT(copy.t(), WithinAbs(2.5, tolerance));
  REQUIRE(&copy.material() == &material);
}

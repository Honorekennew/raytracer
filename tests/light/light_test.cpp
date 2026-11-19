#include <catch2/catch_test_macros.hpp>

#include "raytracer/colour/colour.hpp"
#include "raytracer/light/light.hpp"
#include "raytracer/math/vector.hpp"
#include "test_helpers.hpp"

using raytracer::Colour;
using raytracer::Light;
using raytracer::Vector;

TEST_CASE("Light getters return position and colour", "[light]") {
  Light light(Vector(1, 2, 3), Colour(1, 0.5, 0.25));

  requireVectorsEqual(light.position(), Vector(1, 2, 3));
  requireColoursEqual(light.colour(), Colour(1, 0.5, 0.25));
}

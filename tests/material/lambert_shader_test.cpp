#include <catch2/catch_test_macros.hpp>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/material/flat_shader.hpp"
#include "raytracer/material/lambert_shader.hpp"
#include "raytracer/material/material.hpp"
#include "raytracer/math/vector.hpp"
#include "test_helpers.hpp"

using raytracer::Colour;
using raytracer::FlatShader;
using raytracer::Hit;
using raytracer::LambertShader;
using raytracer::Material;
using raytracer::Ray;
using raytracer::Vector;

// Lambert's law: the colour is  surface colour * (normal . direction to the light), where the
// direction to the light is 1 long, and a negative dot product counts as 0. The light is white.
//
// Most tests look at the point (0, 0, 0) of a surface that faces +Z: its normal is (0, 0, 1).
namespace {

// A Hit needs a material. The shader under test doesn't read it, so any one will do.
const FlatShader hitMaterial(Colour(1, 1, 1));

const Ray anyRay(Vector(0, 0, 10), Vector(0, 0, -1));
const Hit faceUp(10, Vector(0, 0, 0), Vector(0, 0, 1), hitMaterial);

}  // namespace

// The light is straight above the surface: the dot product is 1, the surface keeps its colour.
TEST_CASE("LambertShader gives the full colour when the light faces the surface", "[material]") {
  LambertShader shader(Colour(1, 1, 0));

  requireColoursEqual(shader.shade(anyRay, faceUp, Vector(0, 0, 5)), Colour(1, 1, 0));
}

// toLight is (3, 0, 4), 5 long: the direction is (3, 0, 4) / 5 = (0.6, 0, 0.8). Its dot product
// with the normal (0, 0, 1) is 0.8, so every component of the colour is times 0.8.
TEST_CASE("LambertShader scales the colour by the dot product with the light", "[material]") {
  LambertShader shader(Colour(1, 0.5, 0.25));

  requireColoursEqual(shader.shade(anyRay, faceUp, Vector(3, 0, 4)), Colour(0.8, 0.4, 0.2));
}

// Only the direction counts, not the length of toLight: (3, 0, 4) and (30, 0, 40) point the same
// way.
TEST_CASE("LambertShader does not depend on the length of toLight", "[material]") {
  LambertShader shader(Colour(1, 0.5, 0.25));

  requireColoursEqual(shader.shade(anyRay, faceUp, Vector(30, 0, 40)), Colour(0.8, 0.4, 0.2));
  requireColoursEqual(shader.shade(anyRay, faceUp, Vector(3, 0, 4)), Colour(0.8, 0.4, 0.2));
}

// The light is level with the surface: the direction (1, 0, 0) is at a right angle to the normal,
// the dot product is 0 and the surface stays dark.
TEST_CASE("LambertShader gives black when the light is level with the surface", "[material]") {
  LambertShader shader(Colour(1, 1, 0));

  requireColoursEqual(shader.shade(anyRay, faceUp, Vector(5, 0, 0)), Colour(0, 0, 0));
}

// The light is under the surface: the dot product is -1. A surface can't be darker than black, so
// the negative value counts as 0.
TEST_CASE("LambertShader gives black when the light is behind the surface", "[material]") {
  LambertShader shader(Colour(1, 1, 0));

  requireColoursEqual(shader.shade(anyRay, faceUp, Vector(0, 0, -5)), Colour(0, 0, 0));
}

// toLight is a direction, not a position: it does not matter where the point is. The point
// (0, 0, 10) faces +Z, and a toLight of (0, 0, 5) is above it, whatever the point.
TEST_CASE("LambertShader does not depend on the position of the hit point", "[material]") {
  LambertShader shader(Colour(1, 1, 0));
  Hit high(0, Vector(0, 0, 10), Vector(0, 0, 1), hitMaterial);

  requireColoursEqual(shader.shade(anyRay, high, Vector(0, 0, 5)), Colour(1, 1, 0));
}

// A toLight of (0, 0, 0) has no direction (its length is 0). The result must be black, not "not a
// number".
TEST_CASE("LambertShader gives black when toLight has no length", "[material]") {
  LambertShader shader(Colour(1, 1, 0));

  requireColoursEqual(shader.shade(anyRay, faceUp, Vector(0, 0, 0)), Colour(0, 0, 0));
}

TEST_CASE("LambertShader can be used through a Material reference", "[material]") {
  LambertShader shader(Colour(1, 0.5, 0.25));
  const Material& material = shader;

  requireColoursEqual(material.shade(anyRay, faceUp, Vector(3, 0, 4)), Colour(0.8, 0.4, 0.2));
}

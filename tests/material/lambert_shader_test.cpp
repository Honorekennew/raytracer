#include <catch2/catch_test_macros.hpp>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/hit_info.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/light/light.hpp"
#include "raytracer/material/lambert_shader.hpp"
#include "raytracer/material/material.hpp"
#include "raytracer/math/vector.hpp"
#include "test_helpers.hpp"

using raytracer::Colour;
using raytracer::HitInfo;
using raytracer::LambertShader;
using raytracer::Light;
using raytracer::Material;
using raytracer::Ray;
using raytracer::Vector;

// Lambert's law: the colour is  surface colour * light colour * (normal . direction to the light),
// where the direction to the light is 1 long, and a negative dot product counts as 0.
//
// Most tests look at the point (0, 0, 0) of a surface that faces +Z: its normal is (0, 0, 1).
namespace {

const Ray anyRay(Vector(0, 0, 10), Vector(0, 0, -1));
const HitInfo faceUp(Vector(0, 0, 0), Vector(0, 0, 1));
const Colour white(1, 1, 1);

}  // namespace

// The light is straight above the surface: the dot product is 1, the surface keeps its colour.
TEST_CASE("LambertShader gives the full colour when the light faces the surface", "[material]") {
  LambertShader shader(Colour(1, 1, 0));
  Light light(Vector(0, 0, 5), white);

  requireColoursEqual(shader.shade(anyRay, faceUp, light), Colour(1, 1, 0));
}

// The light is at (3, 0, 4), 5 away from the point: the direction is (3, 0, 4) / 5 = (0.6, 0, 0.8).
// Its dot product with the normal (0, 0, 1) is 0.8, so every component of the colour is times 0.8.
TEST_CASE("LambertShader scales the colour by the dot product with the light", "[material]") {
  LambertShader shader(Colour(1, 0.5, 0.25));
  Light light(Vector(3, 0, 4), white);

  requireColoursEqual(shader.shade(anyRay, faceUp, light), Colour(0.8, 0.4, 0.2));
}

// Only the direction counts, not how far the light is: (0, 0, 1) and (0, 0, 100) are both
// straight above the point.
TEST_CASE("LambertShader does not depend on the distance to the light", "[material]") {
  LambertShader shader(Colour(1, 0.5, 0.25));
  Light closeLight(Vector(3, 0, 4), white);
  Light distantLight(Vector(30, 0, 40), white);

  requireColoursEqual(shader.shade(anyRay, faceUp, distantLight), Colour(0.8, 0.4, 0.2));
  requireColoursEqual(shader.shade(anyRay, faceUp, closeLight), Colour(0.8, 0.4, 0.2));
}

// The light is level with the surface, at (5, 0, 0): the direction (1, 0, 0) is at a right angle
// to the normal, the dot product is 0 and the surface stays dark.
TEST_CASE("LambertShader gives black when the light is level with the surface", "[material]") {
  LambertShader shader(Colour(1, 1, 0));
  Light light(Vector(5, 0, 0), white);

  requireColoursEqual(shader.shade(anyRay, faceUp, light), Colour(0, 0, 0));
}

// The light is under the surface, at (0, 0, -5): the dot product is -1. A surface can't be darker
// than black, so the negative value counts as 0.
TEST_CASE("LambertShader gives black when the light is behind the surface", "[material]") {
  LambertShader shader(Colour(1, 1, 0));
  Light light(Vector(0, 0, -5), white);

  requireColoursEqual(shader.shade(anyRay, faceUp, light), Colour(0, 0, 0));
}

// The direction to the light starts at the point that was hit, not at the origin (0, 0, 0). The
// point (0, 0, 10) faces +Z, and the light at (0, 0, 5) is below it: behind the surface. (Seen
// from the origin, the light would be above.)
TEST_CASE("LambertShader looks at the light from the hit point", "[material]") {
  LambertShader shader(Colour(1, 1, 0));
  HitInfo high(Vector(0, 0, 10), Vector(0, 0, 1));
  Light light(Vector(0, 0, 5), white);

  requireColoursEqual(shader.shade(anyRay, high, light), Colour(0, 0, 0));
}

// The colour of the light is multiplied too: a (0.5, 1, 0) light on a (1, 1, 0.5) surface, with
// the dot product at 1, gives (1 * 0.5, 1 * 1, 0.5 * 0).
TEST_CASE("LambertShader multiplies the surface colour by the light colour", "[material]") {
  LambertShader shader(Colour(1, 1, 0.5));
  Light light(Vector(0, 0, 5), Colour(0.5, 1, 0));

  requireColoursEqual(shader.shade(anyRay, faceUp, light), Colour(0.5, 1, 0));
}

// A light exactly on the point has no direction (its distance is 0). The result must be black, not
// "not a number".
TEST_CASE("LambertShader gives black when the light is on the hit point", "[material]") {
  LambertShader shader(Colour(1, 1, 0));
  Light light(Vector(0, 0, 0), white);

  requireColoursEqual(shader.shade(anyRay, faceUp, light), Colour(0, 0, 0));
}

TEST_CASE("LambertShader can be used through a Material reference", "[material]") {
  LambertShader shader(Colour(1, 0.5, 0.25));
  const Material& material = shader;
  Light light(Vector(3, 0, 4), white);

  requireColoursEqual(material.shade(anyRay, faceUp, light), Colour(0.8, 0.4, 0.2));
}

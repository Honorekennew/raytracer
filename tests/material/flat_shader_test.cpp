#include <catch2/catch_test_macros.hpp>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/light/light.hpp"
#include "raytracer/material/flat_shader.hpp"
#include "raytracer/material/material.hpp"
#include "raytracer/math/vector.hpp"
#include "test_helpers.hpp"

using raytracer::Colour;
using raytracer::FlatShader;
using raytracer::Hit;
using raytracer::Light;
using raytracer::Material;
using raytracer::Ray;
using raytracer::Vector;

namespace {

// A Hit needs a material. The shaders under test don't read it, so any one will do.
const FlatShader hitMaterial(Colour(1, 1, 1));

// A ray that hits the point (0, 0, 4) of a sphere at t = 5, whose normal there is (0, 0, -1).
const Ray straightRay(Vector(0, 0, -1), Vector(0, 0, 1));
const Hit straightHit(5, Vector(0, 0, 4), Vector(0, 0, -1), hitMaterial);
const Light whiteLight(Vector(0, 0, -5), Colour(1, 1, 1));

}  // namespace

TEST_CASE("FlatShader shades with the colour it was created with", "[material]") {
  FlatShader shader(Colour(1, 1, 0));

  requireColoursEqual(shader.shade(straightRay, straightHit, whiteLight), Colour(1, 1, 0));
}

// "Flat" means the same colour everywhere: neither the ray, nor the point, nor the light makes a
// difference.
TEST_CASE("FlatShader gives the same colour wherever the light is", "[material]") {
  FlatShader shader(Colour(0.25, 0.5, 0.75));
  Ray sideways(Vector(3, -2, 5), Vector(1, 1, 0));
  Hit otherHit(1, Vector(4, -1, 5), Vector(0, 1, 0), hitMaterial);
  Light redLight(Vector(100, 100, 100), Colour(1, 0, 0));

  requireColoursEqual(shader.shade(straightRay, straightHit, whiteLight), Colour(0.25, 0.5, 0.75));
  requireColoursEqual(shader.shade(sideways, otherHit, redLight), Colour(0.25, 0.5, 0.75));
}

// The code that uses a material only knows "Material": the call must reach the FlatShader.
TEST_CASE("FlatShader can be used through a Material reference", "[material]") {
  FlatShader shader(Colour(0.25, 0.5, 0.75));
  const Material& material = shader;

  requireColoursEqual(material.shade(straightRay, straightHit, whiteLight),
                      Colour(0.25, 0.5, 0.75));
}

TEST_CASE("Two FlatShaders keep their own colour", "[material]") {
  FlatShader red(Colour(1, 0, 0));
  FlatShader blue(Colour(0, 0, 1));

  requireColoursEqual(red.shade(straightRay, straightHit, whiteLight), Colour(1, 0, 0));
  requireColoursEqual(blue.shade(straightRay, straightHit, whiteLight), Colour(0, 0, 1));
}

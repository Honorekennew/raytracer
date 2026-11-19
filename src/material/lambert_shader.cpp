#include "raytracer/material/lambert_shader.hpp"

#include <algorithm>

#include "raytracer/math/vector.hpp"

namespace raytracer {

namespace {

// The dot product of the normal and the direction to the light, between 0 and 1.
double brightness(const Hit& hit, const Vector& toLight) {
  // A toLight with no length has no direction (we can't divide by a length of 0).
  if (toLight.length() == 0.0) {
    return 0.0;
  }

  // A negative value means the light is behind the surface: no light, not "negative light".
  return std::max(0.0, hit.normal().dot(toLight.normalised()));
}

}  // namespace

LambertShader::LambertShader(const Colour& colour) : _colour(colour) {}

// The ray is not used: it has no name, so that the compiler doesn't warn about it.
Colour LambertShader::shade(const Ray&, const Hit& hit, const Vector& toLight) const {
  double amount = brightness(hit, toLight);
  return Colour(_colour.r() * amount, _colour.g() * amount, _colour.b() * amount);
}

}  // namespace raytracer

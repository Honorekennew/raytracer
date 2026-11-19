#pragma once

#include "raytracer/colour/colour.hpp"
#include "raytracer/math/vector.hpp"

namespace raytracer {

// A point light: a light that comes from a single point of the scene and shines in all
// directions, like a bare light bulb. It has a position and a colour.
class Light {
 public:
  // Creates a light at "position" that shines with "colour".
  Light(const Vector& position, const Colour& colour);

  // Read-only access to the position and the colour.
  // They return a reference ("&") to avoid copying; "const" means the caller cannot change the
  // light through that reference.
  const Vector& position() const;
  const Colour& colour() const;

 private:
  Vector _position;
  Colour _colour;
};

}  // namespace raytracer

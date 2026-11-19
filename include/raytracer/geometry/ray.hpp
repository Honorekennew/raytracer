#pragma once

#include "raytracer/math/vector.hpp"

namespace raytracer {

// A ray: a half-line that starts at a point (the origin) and goes in a direction.
class Ray {
 public:
  // Creates a ray that starts at "origin" and goes towards "direction".
  Ray(const Vector& origin, const Vector& direction);

  // Read-only access to the origin and the direction.
  // They return a reference ("&") to avoid copying the vector; "const" means the caller
  // cannot change it through that reference.
  const Vector& origin() const;
  const Vector& direction() const;

  // Returns the point of the ray at "t": origin + t * direction. At t = 0 it is the origin, and
  // a negative t is behind the origin. Written once here, the shapes use it to find their hit.
  Vector at(double t) const;

 private:
  Vector _origin;
  Vector _direction;
};

}  // namespace raytracer

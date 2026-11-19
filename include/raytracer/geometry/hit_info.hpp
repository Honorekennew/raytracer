#pragma once

#include "raytracer/math/vector.hpp"

namespace raytracer {

// What we learn when a ray hits an object: where it hit, and how the surface is turned there.
// A material needs both to decide the colour.
class HitInfo {
 public:
  // Creates the information about a hit at "point", where the surface has the given "normal".
  // The normal is the direction that sticks out of the surface. It should be 1 long (normalised):
  // the objects that create a HitInfo are responsible for that.
  HitInfo(const Vector& point, const Vector& normal);

  // Read-only access to the point and the normal.
  const Vector& point() const;
  const Vector& normal() const;

 private:
  Vector _point;
  Vector _normal;
};

}  // namespace raytracer

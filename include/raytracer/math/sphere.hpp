#pragma once

#include "raytracer/math/ray.hpp"
#include "raytracer/math/vector.hpp"

namespace raytracer {

// A sphere: all the points at a distance "radius" from a point, the center.
class Sphere {
 public:
  // Creates a sphere with the given center and radius.
  Sphere(const Vector& center, double radius);

  // Read-only access to the center and the radius.
  const Vector& center() const;
  double radius() const;

  // Returns true if the ray touches the sphere.
  // Only the part of the line in front of the ray's origin counts (t >= 0).
  bool hit(const Ray& ray) const;

 private:
  Vector _center;
  double _radius;
};

}  // namespace raytracer

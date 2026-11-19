#pragma once

#include <optional>

#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/ray.hpp"

namespace raytracer {

// An abstract class, like ImageSaver: it says WHAT every shape can do, not HOW. Every shape
// promises the same thing: "hit" returns its nearest hit in front of the ray's origin (the point
// origin + t * direction with t > 0) with a normal of length 1, or nothing if there is none.
class Shape {
 public:
  // A class with virtual functions needs a virtual destructor. Without it, deleting a Sphere
  // through a Shape pointer would not clean up the Sphere part.
  virtual ~Shape() = default;

  // The "= 0" makes the function "pure virtual": there is no code for it here, and every shape
  // must provide its own.
  virtual std::optional<Hit> hit(const Ray& ray) const = 0;
};

}  // namespace raytracer

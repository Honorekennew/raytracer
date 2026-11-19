#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/geometry/shape.hpp"
#include "raytracer/math/vector.hpp"

namespace raytracer {

// A scene: the shapes we look at, and the sun that lights them. With two shapes, a ray can hit
// both, and the pixel must show the nearest: the scene finds it.
class Scene {
 public:
  // Creates a scene with no shapes, lit by a sun in the direction "toLight": the direction that
  // goes from a point of the scene towards the sun. The scene makes it 1 long, like Plane does
  // with its normal: toLight() always gives a unit vector. Only (0, 0, 0), which has no direction,
  // is kept as it is.
  explicit Scene(const Vector& toLight);

  // Read-only access to the direction towards the sun.
  const Vector& toLight() const;

  // Adds a shape to the scene. The scene takes ownership of it: it is the owner and deletes the
  // shape when the scene is deleted. A "std::unique_ptr" has one owner only, so the caller gives
  // its pointer away with "std::move" (or passes the result of "std::make_unique" directly).
  // The shape can be any Shape: a Sphere, a Plane, ...
  void add(std::unique_ptr<Shape> shape);

  // Returns the hit with the smallest t among all the shapes of the scene, which is the one
  // closest to the ray's origin, or nothing if no shape is hit. The order in which the shapes
  // were added makes no difference.
  std::optional<Hit> nearestHit(const Ray& ray) const;

 private:
  Vector _toLight;
  // unique_ptr and not Shape objects: Shape is abstract, so the vector holds pointers, and each
  // pointer reaches a Sphere, a Plane, ... A vector of unique_ptr can't be copied, only moved.
  std::vector<std::unique_ptr<Shape>> _shapes;
};

}  // namespace raytracer

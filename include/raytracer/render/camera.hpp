#pragma once

#include "raytracer/geometry/ray.hpp"
#include "raytracer/math/vector.hpp"

namespace raytracer {

// A camera: the point from which the scene is seen.
// For now it always faces the positive Z-axis, so only its position is stored.
class Camera {
 public:
  // Creates a camera at "position".
  explicit Camera(const Vector& position);

  // Read-only access to the position.
  const Vector& position() const;

  // Returns the ray that starts at the camera and goes straight ahead (towards +Z).
  Ray rayThroughCenter() const;

 private:
  Vector _position;
};

}  // namespace raytracer

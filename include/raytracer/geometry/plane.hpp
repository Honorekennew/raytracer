#pragma once

#include <memory>
#include <optional>

#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/geometry/shape.hpp"
#include "raytracer/material/material.hpp"
#include "raytracer/math/vector.hpp"

namespace raytracer {

// A plane: a flat surface that goes on for ever, like the floor. It is given by a point Q on it
// and its normal n, the direction that sticks out of it. It has a material, which decides the
// colour of the pixels that show it.
//
// A ray O + t * D reaches the plane at t = n . (Q - O) / (n . D). There is no hit when n . D is 0
// (the ray is parallel to the plane) or when t is not above 0 (the plane is behind the ray).
class Plane : public Shape {
 public:
  // Creates a plane through "point", with the given "normal" and "material".
  //
  // The normal does not have to be 1 long: the plane keeps it normalised, and its hits have that
  // normalised normal. It must not be the vector (0, 0, 0), which has no direction.
  Plane(const Vector& point, const Vector& normal, std::shared_ptr<const Material> material);

  // Read-only access to the material.
  const Material& material() const;

  // Returns where the ray hits the plane (the point and the normal), or nothing if the ray is
  // parallel to the plane or if the plane is behind the ray. See Shape for what a hit promises.
  std::optional<Hit> hit(const Ray& ray) const override;

 private:
  Vector _point;
  Vector _normal;
  std::shared_ptr<const Material> _material;
};

}  // namespace raytracer

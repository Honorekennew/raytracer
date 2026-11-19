#pragma once

#include <memory>
#include <optional>

#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/geometry/shape.hpp"
#include "raytracer/material/material.hpp"
#include "raytracer/math/vector.hpp"

namespace raytracer {

// A sphere: all the points at a distance "radius" from a point, the center.
// It has a material, which decides the colour of the pixels that show it.
class Sphere : public Shape {
 public:
  // Creates a sphere with the given center, radius and material.
  //
  // The material is a Material, not a copy of a FlatShader or of another kind: a Material is an
  // abstract class, so the sphere needs a pointer to reach the real object. A shared_ptr is used
  // so that several spheres can share one material, and so that the material lives as long as
  // the last sphere that uses it. The sphere only reads it, hence "const".
  //
  // Throws std::invalid_argument if "material" is null: the sphere could not be shaded.
  Sphere(const Vector& center, double radius, std::shared_ptr<const Material> material);

  // Read-only access to the center, the radius and the material.
  const Vector& center() const;
  double radius() const;
  const Material& material() const;

  // Returns where the ray hits the sphere (the point and the normal), or nothing if it misses.
  // Only the part of the line in front of the ray's origin counts (t > 0): a sphere behind
  // the origin is not hit. If the ray hits the sphere twice, the first hit is returned.
  //
  // "std::optional" is a value that may be empty: use "has_value()", or test it like a bool, to
  // know if there is a hit, and "*hit" or "hit->" to reach the Hit.
  //
  // "override" asks the compiler to check that this really replaces the function of Shape.
  std::optional<Hit> hit(const Ray& ray) const override;

 private:
  // The "t" of the first hit in front of the ray's origin (the point is origin + t * direction),
  // or nothing if the ray misses the sphere.
  std::optional<double> distanceToHit(const Ray& ray) const;

  Vector _center;
  double _radius;
  std::shared_ptr<const Material> _material;
};

}  // namespace raytracer

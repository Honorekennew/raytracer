#pragma once

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/material/material.hpp"
#include "raytracer/math/vector.hpp"

namespace raytracer {

// The simplest Material: the same colour everywhere, whatever the light or the angle.
class FlatShader : public Material {
 public:
  // Creates a material that always gives "colour".
  explicit FlatShader(const Colour& colour);

  // Returns the colour given to the constructor, whatever the ray, the point and the light.
  //
  // "override" asks the compiler to check that this really replaces the function of Material:
  // if the name or the parameters differ, it is a compile error, not a silent new function.
  Colour shade(const Ray& ray, const Hit& hit, const Vector& toLight) const override;

 private:
  Colour _colour;
};

}  // namespace raytracer

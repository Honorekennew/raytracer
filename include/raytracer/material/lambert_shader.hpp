#pragma once

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/hit_info.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/light/light.hpp"
#include "raytracer/material/material.hpp"

namespace raytracer {

// A Material that applies Lambert's law: a surface is brighter when it faces the light, and darker
// when it is turned away from it. It is how a matte surface (chalk, paper, ...) looks.
//
// The brightness is the dot product of the normal of the surface and the direction to the light
// (both 1 long): 1 when the light is straight above the surface, 0 when it is level with it.
class LambertShader : public Material {
 public:
  // Creates a material whose surface has the given "colour" when the light faces it.
  explicit LambertShader(const Colour& colour);

  // Returns the colour of the surface multiplied by the colour of the light and by the
  // brightness. The brightness is never below 0: a surface turned away from the light is black,
  // not "darker than black". The ray is not used.
  Colour shade(const Ray& ray, const HitInfo& hit, const Light& light) const override;

 private:
  Colour _colour;
};

}  // namespace raytracer

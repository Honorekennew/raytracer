#pragma once

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/hit.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/math/vector.hpp"

namespace raytracer {

// An abstract class (the Strategy pattern, like ImageSaver): it only says WHAT a material can do,
// not HOW. A material decides the colour of a point of an object. Each way of colouring (one flat
// colour, light and shadow, a chessboard, ...) is a class that inherits from Material and writes
// the "shade" function. The code that draws an object only has a Material: it doesn't need to
// know which one.
class Material {
 public:
  // A class with virtual functions needs a virtual destructor. Without it, deleting a FlatShader
  // through a Material pointer would not clean up the FlatShader part.
  virtual ~Material() = default;

  // Returns the colour to draw where the "ray" hit the object, given what we know about the hit
  // ("hit": the point and the normal) and the direction to the sun ("toLight": it starts at the
  // hit point and points to the sun; its length does not matter, and the sun is white).
  //
  // The "= 0" makes the function "pure virtual": there is no code for it here, and every class
  // that inherits from Material must provide its own. Each material uses what it needs: a
  // FlatShader uses none of the parameters, a LambertShader uses the hit and toLight.
  //
  // All the parameters are "const &": a material reads them, it never changes them.
  virtual Colour shade(const Ray& ray, const Hit& hit, const Vector& toLight) const = 0;
};

}  // namespace raytracer

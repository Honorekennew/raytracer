#pragma once

#include "raytracer/math/vector.hpp"

namespace raytracer {

// Material needs a Hit to shade, and a Hit holds a Material: declaring the name here, without
// including material.hpp, avoids two headers that include each other.
class Material;

// What we learn when a ray hits an object: how far along the ray, where, how the surface is turned
// there, and what it is made of. The scene uses "t" to keep the nearest of several hits, and the
// material to decide the colour.
class Hit {
 public:
  // Creates the information about a hit.
  //  - "t": the hit is at the point origin + t * direction of the ray.
  //  - "point": that point.
  //  - "normal": the direction that sticks out of the surface. It should be 1 long (normalised):
  //    the objects that create a Hit are responsible for that.
  //  - "material": the material of the object that was hit. The Hit does not own it: it only
  //    points to it, so the object (or whoever owns the material) must live longer than the Hit.
  Hit(double t, const Vector& point, const Vector& normal, const Material& material);

  // Read-only access to what the hit holds.
  double t() const;
  const Vector& point() const;
  const Vector& normal() const;
  const Material& material() const;

 private:
  double _t;
  Vector _point;
  Vector _normal;
  // A pointer, not a reference: a reference member would make the Hit impossible to assign, and
  // a scene needs to replace the nearest hit it has found so far by a nearer one.
  const Material* _material;
};

}  // namespace raytracer

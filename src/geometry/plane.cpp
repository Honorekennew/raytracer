#include "raytracer/geometry/plane.hpp"

#include <utility>

namespace raytracer {

// The normal is made 1 long here, once, so that every hit can give it as it is.
Plane::Plane(const Vector& point, const Vector& normal, std::shared_ptr<const Material> material)
    : _point(point), _normal(normal.normalised()), _material(std::move(material)) {}

const Material& Plane::material() const { return *_material; }

// A point of the ray is: origin + t * direction.
// It is on the plane when (point - Q) . n = 0, which gives t = n . (Q - origin) / (n . direction).
std::optional<Hit> Plane::hit(const Ray& ray) const {
  // n . D is 0 when the ray runs along the plane, at a right angle to the normal. The ray never
  // reaches the plane (or stays in it), so there is no hit, and we can't divide by 0.
  double alongNormal = _normal.dot(ray.direction());
  if (alongNormal == 0.0) {
    return std::nullopt;
  }

  // The plane must be in front of the origin: t > 0, as every shape promises.
  double t = _normal.dot(_point - ray.origin()) / alongNormal;
  if (t <= 0.0) {
    return std::nullopt;
  }

  return Hit(t, ray.at(t), _normal, *_material);
}

}  // namespace raytracer

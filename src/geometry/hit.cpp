#include "raytracer/geometry/hit.hpp"

namespace raytracer {

Hit::Hit(const Vector& point, const Vector& normal) : _point(point), _normal(normal) {}

const Vector& Hit::point() const { return _point; }

const Vector& Hit::normal() const { return _normal; }

}  // namespace raytracer

#include "raytracer/geometry/hit_info.hpp"

namespace raytracer {

HitInfo::HitInfo(const Vector& point, const Vector& normal) : _point(point), _normal(normal) {}

const Vector& HitInfo::point() const { return _point; }

const Vector& HitInfo::normal() const { return _normal; }

}  // namespace raytracer

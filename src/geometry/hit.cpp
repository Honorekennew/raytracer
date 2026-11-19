#include "raytracer/geometry/hit.hpp"

namespace raytracer {

Hit::Hit(double t, const Vector& point, const Vector& normal, const Material& material)
    : _t(t), _point(point), _normal(normal), _material(&material) {}

double Hit::t() const { return _t; }

const Vector& Hit::point() const { return _point; }

const Vector& Hit::normal() const { return _normal; }

const Material& Hit::material() const { return *_material; }

}  // namespace raytracer

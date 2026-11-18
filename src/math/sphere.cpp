#include "raytracer/math/sphere.hpp"

#include <cmath>

namespace raytracer {

Sphere::Sphere(const Vector& center, double radius) : _center(center), _radius(radius) {}

const Vector& Sphere::center() const { return _center; }

double Sphere::radius() const { return _radius; }

// A point of the ray is: origin + t * direction (t >= 0).
// It is on the sphere when its distance to the center equals the radius.
// This gives a quadratic equation a*t^2 + b*t + c = 0, whose solutions are the hits.
bool Sphere::hit(const Ray& ray) const {
  Vector d = ray.direction();
  Vector l = ray.origin() - _center;

  double a = d.dot(d);
  double b = 2.0 * d.dot(l);
  double c = l.dot(l) - _radius * _radius;

  // No real solution: the ray misses the sphere.
  double delta = b * b - 4.0 * a * c;
  if (delta < 0) {
    return false;
  }

  double t1 = (-b + std::sqrt(delta)) / (2.0 * a);
  double t2 = (-b - std::sqrt(delta)) / (2.0 * a);

  // At least one hit must be in front of the origin.
  return t1 >= 0 || t2 >= 0;
}

}  // namespace raytracer

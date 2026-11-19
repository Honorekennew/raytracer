#include "raytracer/geometry/sphere.hpp"

#include <cmath>
#include <optional>
#include <stdexcept>
#include <utility>

namespace raytracer {

Sphere::Sphere(const Vector& center, double radius, std::shared_ptr<const Material> material)
    : _center(center), _radius(radius), _material(std::move(material)) {
  if (_material == nullptr) {
    throw std::invalid_argument("Sphere: the material must not be null");
  }
}

const Vector& Sphere::center() const { return _center; }

double Sphere::radius() const { return _radius; }

const Material& Sphere::material() const { return *_material; }

// A point of the ray is: origin + t * direction.
// It is on the sphere when its distance to the center equals the radius.
// This gives a quadratic equation a*t^2 + b*t + c = 0, whose solutions are the hits.
// Returns the "t" of the first hit in front of the origin, or nothing if the ray misses.
std::optional<double> Sphere::distanceToHit(const Ray& ray) const {
  Vector d = ray.direction();
  Vector l = ray.origin() - _center;

  double a = d.dot(d);
  double b = 2.0 * d.dot(l);
  double c = l.dot(l) - _radius * _radius;

  // No real solution: the ray misses the sphere.
  double discriminant = b * b - 4.0 * a * c;
  if (discriminant < 0.0) {
    return std::nullopt;
  }

  // t1 is the smaller solution: the point where the ray enters the sphere.
  double root = std::sqrt(discriminant);
  double t1 = (-b - root) / (2.0 * a);
  double t2 = (-b + root) / (2.0 * a);

  // The first hit that is in front of the origin counts. If the ray starts inside the sphere,
  // t1 is behind the origin and the hit is where the ray leaves the sphere (t2).
  if (t1 > 0.0) {
    return t1;
  }
  if (t2 > 0.0) {
    return t2;
  }
  return std::nullopt;
}

std::optional<Hit> Sphere::hit(const Ray& ray) const {
  std::optional<double> t = distanceToHit(ray);
  if (!t.has_value()) {
    return std::nullopt;
  }

  // The point of the ray at "t", and the normal: from the center to that point, made 1 long.
  Vector point = ray.origin() + *t * ray.direction();
  Vector normal = (point - _center).normalised();
  return Hit(point, normal);
}

}  // namespace raytracer

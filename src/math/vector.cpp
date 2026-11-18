#include "raytracer/math/vector.hpp"

namespace raytracer {

// The part after the ":" gives the starting value of each component.
Vector::Vector() : _x(0.0), _y(0.0), _z(0.0) {}

Vector::Vector(double x, double y, double z) : _x(x), _y(y), _z(z) {}

Vector Vector::operator+(const Vector& other) const {
  // A function of the class may read the private components of another Vector too:
  // "private" protects the class from the outside, not one object from another.
  return Vector(_x + other._x, _y + other._y, _z + other._z);
}

Vector Vector::operator-(const Vector& other) const {
  return Vector(_x - other._x, _y - other._y, _z - other._z);
}

double Vector::dot(const Vector& other) const {
  return _x * other._x + _y * other._y + _z * other._z;
}

double Vector::x() const { return _x; }

double Vector::y() const { return _y; }

double Vector::z() const { return _z; }

}  // namespace raytracer

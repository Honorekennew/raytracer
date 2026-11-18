#pragma once

namespace raytracer {

// A 3D vector: three numbers x, y and z.
// It can describe a point or a direction in 3D space.
class Vector {
 public:
  // Creates the vector (0, 0, 0).
  Vector();

  // Creates the vector (x, y, z).
  Vector(double x, double y, double z);

  // Returns a new vector equal to this vector plus "other", component by component.
  // Neither of the two original vectors is changed.
  // Usage: Vector v3 = v1 + v2;
  //
  // "operator+" is the function the compiler calls when you write "v1 + v2".
  // "const Vector&" means: use "other" without making a copy and without changing it.
  // The final "const" means: this function does not change the vector it is called on.
  Vector operator+(const Vector& other) const;

  // Returns a new vector equal to this vector minus "other", component by component.
  // Neither of the two original vectors is changed.
  // Usage: Vector v3 = v1 - v2;
  Vector operator-(const Vector& other) const;

  // Returns the dot product of this vector and "other":
  // x * other.x + y * other.y + z * other.z.
  double dot(const Vector& other) const;

  // Read-only access to the components. They let you read a value but not change it.
  double x() const;
  double y() const;
  double z() const;

 private:
  // The three components. Only the functions of this class can touch them.
  // The leading "_" is our naming rule for private variables.
  double _x;
  double _y;
  double _z;
};

}  // namespace raytracer

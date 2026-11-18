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

  // Returns a new vector with each component multiplied by "scalar".
  // Usage: Vector v2 = v1 * 2.0;   (the other order, 2.0 * v1, is declared below the class)
  Vector operator*(double scalar) const;

  // Returns the length of the vector: the square root of x*x + y*y + z*z.
  double length() const;

  // Returns a new vector that points the same way and has length 1.
  // The vector must not be (0, 0, 0): its length is 0, and we can't divide by 0.
  Vector normalised() const;

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

// The same multiplication with the number first: Vector v2 = 2.0 * v1;
// It is not a member of the class: a member function always has the vector on its left.
Vector operator*(double scalar, const Vector& vector);

}  // namespace raytracer

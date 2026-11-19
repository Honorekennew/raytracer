#pragma once

namespace raytracer {

// A colour: three numbers r (red), g (green) and b (blue).
class Colour {
 public:
  // Creates the colour (r, g, b).
  Colour(double r, double g, double b);

  // Read-only access to the components.
  double r() const;
  double g() const;
  double b() const;

 private:
  double _r;
  double _g;
  double _b;
};

}  // namespace raytracer

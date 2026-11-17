#include <iostream>

#include "raytracer/demo/greeting.hpp"
#include "raytracer/math/vector.hpp"

int main(int argc, char* argv[]) {
  const char* name = (argc > 1) ? argv[1] : "World";
  std::cout << raytracer::greeting(name) << '\n';

  // Quick manual test of the vector operators.
  raytracer::Vector v1(1.0, 2.0, 3.0);
  raytracer::Vector v2(4.0, 5.0, 6.0);
  raytracer::Vector v3 = v1 + v2;
  raytracer::Vector v4 = v1 - v2;

  std::cout << "v1 + v2 = (" << v3.x() << ", " << v3.y() << ", " << v3.z() << ")\n";
  std::cout << "v1 - v2 = (" << v4.x() << ", " << v4.y() << ", " << v4.z() << ")\n";
  return 0;
}

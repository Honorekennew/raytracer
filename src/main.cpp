#include <iostream>

#include "raytracer/demo/greeting.hpp"

int main(int argc, char* argv[]) {
  const char* name = (argc > 1) ? argv[1] : "World";
  std::cout << raytracer::greeting(name) << '\n';
  return 0;
}

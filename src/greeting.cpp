#include "raytracer/greeting.hpp"

namespace raytracer {

std::string greeting(std::string_view name) {
  if (name.empty()) {
    return "Hello!";
  }
  return "Hello, " + std::string(name) + "!";
}

}  // namespace raytracer

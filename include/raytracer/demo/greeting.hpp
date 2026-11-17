#pragma once

#include <string>
#include <string_view>

namespace raytracer {

/// Returns the greeting printed by the demo, e.g. "Hello, World!".
/// An empty name gives a plain "Hello!".
std::string greeting(std::string_view name);

}  // namespace raytracer

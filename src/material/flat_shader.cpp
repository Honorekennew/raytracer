#include "raytracer/material/flat_shader.hpp"

namespace raytracer {

FlatShader::FlatShader(const Colour& colour) : _colour(colour) {}

// The parameters are not used: they have no name, so that the compiler doesn't warn about them.
Colour FlatShader::shade(const Ray&, const Hit&, const Light&) const { return _colour; }

}  // namespace raytracer

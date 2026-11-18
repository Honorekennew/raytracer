#pragma once

#include "raytracer/colour/colour.hpp"
#include "raytracer/math/vector.hpp"

// Helpers shared by the tests. Real numbers calculated by the code are compared with a small
// tolerance, not with ==.
extern const double tolerance;

void requireVectorsEqual(const raytracer::Vector& actual, const raytracer::Vector& expected);
void requireColoursEqual(const raytracer::Colour& actual, const raytracer::Colour& expected);

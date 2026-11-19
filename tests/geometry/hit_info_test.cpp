#include <catch2/catch_test_macros.hpp>

#include "raytracer/geometry/hit_info.hpp"
#include "raytracer/math/vector.hpp"
#include "test_helpers.hpp"

using raytracer::HitInfo;
using raytracer::Vector;

TEST_CASE("HitInfo getters return point and normal", "[hit_info]") {
  HitInfo hit(Vector(1, 2, 3), Vector(0, 0, 1));

  requireVectorsEqual(hit.point(), Vector(1, 2, 3));
  requireVectorsEqual(hit.normal(), Vector(0, 0, 1));
}

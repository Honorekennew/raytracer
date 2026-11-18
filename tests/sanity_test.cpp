#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "test_helpers.hpp"

using Catch::Matchers::WithinAbs;

TEST_CASE("Sanity check", "[sanity]") { REQUIRE(1 + 1 == 2); }

// 0.1 and 0.2 can't be stored exactly in binary, so 0.1 + 0.2 is not exactly 0.3.
// This is why tests compare real numbers with a small tolerance, not with ==.
TEST_CASE("Floating point equality", "[sanity]") {
  double sum = 0.1 + 0.2;

  REQUIRE(sum != 0.3);
  REQUIRE_THAT(sum, WithinAbs(0.3, tolerance));
}

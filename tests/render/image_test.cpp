#include <catch2/catch_test_macros.hpp>

#include "raytracer/colour/colour.hpp"
#include "raytracer/render/image.hpp"
#include "test_helpers.hpp"

using raytracer::Colour;
using raytracer::Image;

// The image is 5 columns wide and 4 rows high, so that a column and a row can't be mixed up:
// swapping x and y would set the pixel (2, 3) instead of (3, 2).
TEST_CASE("setPixel sets one pixel, all others remain black", "[image]") {
  Image image(5, 4);
  Colour red(1, 0, 0);

  image.setPixel(3, 2, red);

  for (unsigned int y = 0; y < 4; ++y) {
    for (unsigned int x = 0; x < 5; ++x) {
      if (x == 3 && y == 2) {
        requireColoursEqual(image.pixel(x, y), red);
      } else {
        requireColoursEqual(image.pixel(x, y), Colour(0, 0, 0));
      }
    }
  }
}

#pragma once

#include <vector>

#include "raytracer/colour/colour.hpp"

namespace raytracer {

// An image: a grid of pixels, each with a Colour.
//
// A pixel is found by its column "x" and its row "y". The pixel (0, 0) is at the top left:
// x grows to the right and y grows downwards.
class Image {
 public:
  // Creates an image of "width" columns and "height" rows. All the pixels start black.
  Image(unsigned int width, unsigned int height);

  // Read-only access to the size of the image.
  unsigned int width() const;
  unsigned int height() const;

  // Sets the pixel at column "x" and row "y" to "colour".
  // "x" must be less than width() and "y" less than height(): this is not checked yet.
  void setPixel(unsigned int x, unsigned int y, const Colour& colour);

  // Returns the colour of the pixel at column "x" and row "y" (same limits as setPixel).
  const Colour& pixel(unsigned int x, unsigned int y) const;

 private:
  unsigned int _width;
  unsigned int _height;

  // The colours of all the pixels, one row after the other: the pixel (x, y) is at the
  // position y * width + x. A std::vector is an array that knows its own size.
  std::vector<Colour> _pixels;
};

}  // namespace raytracer

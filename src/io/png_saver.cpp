#include "raytracer/io/png_saver.hpp"

#include <lodepng.h>

#include <cmath>
#include <stdexcept>
#include <vector>

namespace raytracer {

namespace {

// Turns a colour component from the range 0 to 1 into a byte from 0 to 255.
// Below 0 gives 0 and above 1 gives 255. "!(value > 0.0)" also catches NaN ("not a number"),
// which would otherwise not be a valid number to round.
unsigned char toByte(double value) {
  if (!(value > 0.0)) {
    return 0;
  }
  if (value >= 1.0) {
    return 255;
  }
  return static_cast<unsigned char>(std::lround(value * 255.0));
}

}  // namespace

void PngSaver::save(const Image& image, const std::string& path) {
  // A PNG needs at least one pixel. LodePNG would write a file for an empty image anyway, and
  // that file would not be a valid PNG, so we refuse it here.
  if (image.width() == 0 || image.height() == 0) {
    throw std::runtime_error("PngSaver: cannot save an image with no pixels to \"" + path + "\"");
  }

  // LodePNG wants the pixels as bytes: red, green and blue of each pixel, row by row from the
  // top left. That is the order our Image uses too, so we read the pixels the same way.
  std::vector<unsigned char> bytes;
  bytes.reserve(static_cast<std::size_t>(image.width()) * image.height() * 3);
  for (unsigned int y = 0; y < image.height(); ++y) {
    for (unsigned int x = 0; x < image.width(); ++x) {
      const Colour& colour = image.pixel(x, y);
      bytes.push_back(toByte(colour.r()));
      bytes.push_back(toByte(colour.g()));
      bytes.push_back(toByte(colour.b()));
    }
  }

  // LodePNG returns 0 when it works and an error number otherwise. We turn the number into an
  // exception with LodePNG's own message (for example, when the folder doesn't exist).
  unsigned error = lodepng::encode(path, bytes, image.width(), image.height(), LCT_RGB, 8);
  if (error != 0) {
    throw std::runtime_error("PngSaver: cannot save \"" + path + "\": " + lodepng_error_text(error));
  }
}

}  // namespace raytracer

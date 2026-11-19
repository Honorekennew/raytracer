#include <lodepng.h>

#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

#include "raytracer/colour/colour.hpp"
#include "raytracer/io/image_saver.hpp"
#include "raytracer/io/png_saver.hpp"
#include "raytracer/render/image.hpp"

using raytracer::Colour;
using raytracer::Image;
using raytracer::ImageSaver;
using raytracer::PngSaver;

namespace {

// A path in the folder where the tests run (the build folder), not in the shared temporary folder
// of the machine, where other users could replace the file. The file is deleted when the test ends,
// even if it fails.
class TempFile {
 public:
  explicit TempFile(const std::string& name) : _path(name) {}
  ~TempFile() {
    std::error_code ignored;
    std::filesystem::remove(_path, ignored);
  }

  // The object owns the file: a copy would delete it a second time.
  TempFile(const TempFile&) = delete;
  TempFile& operator=(const TempFile&) = delete;
  std::string path() const { return _path.string(); }

 private:
  std::filesystem::path _path;
};

// Reads a PNG back as 8-bit red, green and blue: 3 bytes for each pixel, row by row.
std::vector<unsigned char> readPng(const std::string& path, unsigned& width, unsigned& height) {
  std::vector<unsigned char> bytes;
  unsigned error = lodepng::decode(bytes, width, height, path, LCT_RGB, 8);
  REQUIRE(error == 0);
  return bytes;
}

// The three bytes of the pixel (x, y) in an image that is "width" pixels wide.
std::vector<unsigned char> pixelBytes(const std::vector<unsigned char>& bytes, unsigned width,
                                      unsigned x, unsigned y) {
  std::size_t start = 3 * (static_cast<std::size_t>(y) * width + x);
  return {bytes[start], bytes[start + 1], bytes[start + 2]};
}

using Bytes = std::vector<unsigned char>;

}  // namespace

// The image is 3 columns wide and 2 rows high. 1 becomes 255, 0 stays 0, and
// 0.2 * 255 = 51, 0.4 * 255 = 102, 0.6 * 255 = 153.
TEST_CASE("PngSaver writes a PNG with the size and the colours of the image", "[png_saver]") {
  Image image(3, 2);
  image.setPixel(0, 0, Colour(1, 0, 0));
  image.setPixel(2, 1, Colour(0.2, 0.4, 0.6));
  TempFile file("raytracer_png_saver_colours.png");
  PngSaver pngSaver;
  ImageSaver& saver = pngSaver;

  saver.save(image, file.path());

  unsigned width = 0;
  unsigned height = 0;
  Bytes bytes = readPng(file.path(), width, height);
  REQUIRE(width == 3);
  REQUIRE(height == 2);
  REQUIRE(pixelBytes(bytes, width, 0, 0) == Bytes{255, 0, 0});
  REQUIRE(pixelBytes(bytes, width, 2, 1) == Bytes{51, 102, 153});
  // All the other pixels are black.
  REQUIRE(pixelBytes(bytes, width, 1, 0) == Bytes{0, 0, 0});
  REQUIRE(pixelBytes(bytes, width, 2, 0) == Bytes{0, 0, 0});
  REQUIRE(pixelBytes(bytes, width, 0, 1) == Bytes{0, 0, 0});
  REQUIRE(pixelBytes(bytes, width, 1, 1) == Bytes{0, 0, 0});
}

TEST_CASE("PngSaver saves a colour component below 0 as 0 and above 1 as 255", "[png_saver]") {
  Image image(1, 1);
  image.setPixel(0, 0, Colour(2, -1, 0));
  TempFile file("raytracer_png_saver_clamp.png");
  PngSaver saver;

  saver.save(image, file.path());

  unsigned width = 0;
  unsigned height = 0;
  Bytes bytes = readPng(file.path(), width, height);
  REQUIRE(pixelBytes(bytes, width, 0, 0) == Bytes{255, 0, 0});
}

TEST_CASE("PngSaver overwrites an existing file", "[png_saver]") {
  TempFile file("raytracer_png_saver_overwrite.png");
  PngSaver saver;
  Image first(1, 1);
  first.setPixel(0, 0, Colour(1, 1, 1));
  saver.save(first, file.path());

  Image second(2, 1);
  saver.save(second, file.path());

  unsigned width = 0;
  unsigned height = 0;
  Bytes bytes = readPng(file.path(), width, height);
  REQUIRE(width == 2);
  REQUIRE(height == 1);
}

TEST_CASE("PngSaver throws when the file can't be written", "[png_saver]") {
  Image image(1, 1);
  std::filesystem::path path = std::filesystem::path("raytracer_no_such_folder") / "image.png";
  PngSaver saver;

  REQUIRE_THROWS_AS(saver.save(image, path.string()), std::runtime_error);
}

TEST_CASE("PngSaver throws when the image has no pixels", "[png_saver]") {
  Image image(0, 0);
  TempFile file("raytracer_png_saver_empty.png");
  PngSaver saver;

  REQUIRE_THROWS_AS(saver.save(image, file.path()), std::runtime_error);
}

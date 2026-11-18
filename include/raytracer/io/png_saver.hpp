#pragma once

#include <string>

#include "raytracer/io/image_saver.hpp"
#include "raytracer/render/image.hpp"

namespace raytracer {

// An ImageSaver that encodes the image as a PNG (with the LodePNG library) and writes it to disk.
class PngSaver : public ImageSaver {
 public:
  // Saves the image as a PNG file at "path", with 8 bits for each of red, green and blue.
  //
  // Each colour component goes from 0 to 1 in the image, and from 0 to 255 in the file:
  // 0 becomes 0, 1 becomes 255, and the values in between are rounded to the nearest whole
  // number. A component below 0 is saved as 0 and a component above 1 as 255.
  //
  // An existing file at "path" is overwritten. Throws std::runtime_error if the file can't be
  // written (for example, the folder doesn't exist) or if the image has no pixels.
  //
  // "override" asks the compiler to check that this really replaces the function of ImageSaver:
  // if the name or the parameters differ, it is a compile error, not a silent new function.
  void save(const Image& image, const std::string& path) override;
};

}  // namespace raytracer

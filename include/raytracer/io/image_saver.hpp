#pragma once

#include <string>

#include "raytracer/render/image.hpp"

namespace raytracer {

// An abstract class: it only says WHAT a saver can do, not HOW. You can't create an ImageSaver
// directly. Each way of saving an image (PNG, PPM, ...) is a class that inherits from it and
// writes the "save" function. Code that has an ImageSaver doesn't need to know which format
// it writes.
class ImageSaver {
 public:
  // A class with virtual functions needs a virtual destructor. Without it, deleting a PngSaver
  // through an ImageSaver pointer would not clean up the PngSaver part.
  virtual ~ImageSaver() = default;

  // Saves the image to the file at "path". The "= 0" makes the function "pure virtual": there
  // is no code for it here, and every class that inherits from ImageSaver must provide its own.
  //
  // The image is taken as "const Image&": a saver reads the image, it never changes it.
  // An existing file at "path" is overwritten. If the image can't be saved, the function
  // throws an exception.
  virtual void save(const Image& image, const std::string& path) = 0;
};

}  // namespace raytracer

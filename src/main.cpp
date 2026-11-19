#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/sphere.hpp"
#include "raytracer/io/png_saver.hpp"
#include "raytracer/light/light.hpp"
#include "raytracer/material/lambert_shader.hpp"
#include "raytracer/math/vector.hpp"
#include "raytracer/render/camera.hpp"
#include "raytracer/render/image.hpp"

namespace {

// Saves the image as a PNG, in the folder where the program is started.
bool save(const raytracer::Image& image, const std::string& path) {
  try {
    raytracer::PngSaver saver;
    saver.save(image, path);
    return true;
  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return false;
  }
}

}  // namespace

int main() {
  // Position and configure the camera. The screen is 2 units wide: the view is 90 degrees wide.
  raytracer::Camera camera(raytracer::Vector(0, 0, -1), 2);

  // Describe the scene: a yellow sphere with Lambert shading, and a white light above it, on the
  // left and in front of it (towards the camera).
  auto yellow = std::make_shared<raytracer::LambertShader>(raytracer::Colour(1, 1, 0));
  raytracer::Sphere sphere(raytracer::Vector(0, 0, 9), 5, yellow);
  raytracer::Light light(raytracer::Vector(-10, 10, -10), raytracer::Colour(1, 1, 1));

  // Setup memory buffer for the final image
  const unsigned int imageWidth = 512;
  const unsigned int imageHeight = 512;
  raytracer::Image image(imageWidth, imageHeight);

  // Render the scene into the image
  camera.render(sphere, light, image);

  // Save the rendered image to a file
  return save(image, "output.png") ? 0 : 1;
}

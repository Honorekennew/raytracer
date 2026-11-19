#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/plane.hpp"
#include "raytracer/geometry/sphere.hpp"
#include "raytracer/io/png_saver.hpp"
#include "raytracer/material/lambert_shader.hpp"
#include "raytracer/math/vector.hpp"
#include "raytracer/render/camera.hpp"
#include "raytracer/render/image.hpp"
#include "raytracer/scene/scene.hpp"

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

  // Describe the scene. The sun is above, on the left and in front (towards the camera): toLight
  // is the direction from a point of the scene towards it. The scene makes it 1 long.
  raytracer::Scene scene(raytracer::Vector(-1, 1, -1));

  // A red sphere with Lambert shading...
  auto red = std::make_shared<raytracer::LambertShader>(raytracer::Colour(1, 0, 0));
  scene.add(std::make_unique<raytracer::Sphere>(raytracer::Vector(0, 0, 9), 5, red));

  // ...and a grey floor under it: the plane y = -5, which touches the bottom of the sphere.
  auto grey = std::make_shared<raytracer::LambertShader>(raytracer::Colour(0.8, 0.8, 0.8));
  scene.add(std::make_unique<raytracer::Plane>(raytracer::Vector(0, -5, 0),
                                               raytracer::Vector(0, 1, 0), grey));

  // Render the scene into an image
  const unsigned int imageWidth = 512;
  const unsigned int imageHeight = 512;
  raytracer::Image image = camera.render(scene, imageWidth, imageHeight);

  // Save the rendered image to a file, and say where it is
  const std::string path = "image.png";
  if (!save(image, path)) {
    return 1;
  }
  std::cout << path << '\n';
  return 0;
}

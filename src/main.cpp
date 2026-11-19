#include <exception>
#include <iostream>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/geometry/sphere.hpp"
#include "raytracer/io/png_saver.hpp"
#include "raytracer/math/vector.hpp"
#include "raytracer/render/camera.hpp"
#include "raytracer/render/image.hpp"

int main() {
  std::cout << "Hello, Raytracer!\n";

  // The screen is 2 units wide, so the camera sees 90 degrees from left to right.
  raytracer::Camera camera(raytracer::Vector(0, 0, -1), 2);

  // Sphere: center and radius
  raytracer::Sphere sphere(raytracer::Vector(0, 0, 9), 5);

  // An 800 x 600 image. For each pixel, we send a ray through it: the pixel is red if the ray
  // hits the sphere, and black otherwise.
  const unsigned int width = 800;
  const unsigned int height = 600;
  raytracer::Image image(width, height);
  const raytracer::Colour red(1, 0, 0);
  for (unsigned int row = 0; row < height; ++row) {
    for (unsigned int column = 0; column < width; ++column) {
      raytracer::Ray ray = camera.rayThroughPixel(column, row, width, height);
      if (sphere.hit(ray)) {
        image.setPixel(column, row, red);
      }
    }
  }

  // The colour of the pixel in the middle of the image.
  const raytracer::Colour& centre = image.pixel(width / 2, height / 2);
  std::cout << "Colour: (" << centre.r() << ", " << centre.g() << ", " << centre.b() << ")\n";

  // Save the image as a PNG, in the folder where the program is started.
  try {
    raytracer::PngSaver saver;
    saver.save(image, "output.png");
    std::cout << "Saved output.png\n";
  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }

  return 0;
}

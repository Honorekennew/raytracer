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

  raytracer::Camera camera(raytracer::Vector(0, 0, -1));
  raytracer::Ray ray = camera.rayThroughCenter();

  // Sphere: center and radius
  raytracer::Sphere sphere(raytracer::Vector(0, 0, 9), 5);

  // Black by default, red if the ray hits the sphere.
  raytracer::Colour colour(0, 0, 0);
  bool hit = sphere.hit(ray);
  if (hit) {
    colour = raytracer::Colour(1, 0, 0);
  }

  std::cout << "Colour: (" << colour.r() << ", " << colour.g() << ", " << colour.b() << ")\n";

  // An 800 x 600 image, with the pixel at its centre set to the colour we just computed.
  raytracer::Image image(800, 600);
  image.setPixel(400, 300, colour);

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

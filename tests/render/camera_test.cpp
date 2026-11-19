#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <memory>

#include "raytracer/colour/colour.hpp"
#include "raytracer/geometry/ray.hpp"
#include "raytracer/geometry/sphere.hpp"
#include "raytracer/light/light.hpp"
#include "raytracer/material/flat_shader.hpp"
#include "raytracer/material/lambert_shader.hpp"
#include "raytracer/math/vector.hpp"
#include "raytracer/render/camera.hpp"
#include "raytracer/render/image.hpp"
#include "test_helpers.hpp"

using Catch::Matchers::WithinAbs;
using raytracer::Camera;
using raytracer::Colour;
using raytracer::FlatShader;
using raytracer::Image;
using raytracer::LambertShader;
using raytracer::Light;
using raytracer::Ray;
using raytracer::Sphere;
using raytracer::Vector;

namespace {

// A sphere that is one flat colour.
Sphere flatSphere(const Vector& center, double radius, const Colour& colour) {
  return Sphere(center, radius, std::make_shared<FlatShader>(colour));
}

// A white light in front of the camera. It does not change the flat spheres.
Light whiteLight() { return Light(Vector(0, 0, -5), Colour(1, 1, 1)); }

}  // namespace

TEST_CASE("Camera getters return position and screen width", "[camera]") {
  Camera camera(Vector(1, 2, 3), 4.5);

  requireVectorsEqual(camera.position(), Vector(1, 2, 3));
  REQUIRE_THAT(camera.screenWidth(), WithinAbs(4.5, tolerance));
}

TEST_CASE("Ray through center has camera position as origin", "[camera]") {
  Camera camera(Vector(1, 2, 3), 2);

  Ray ray = camera.rayThroughCenter();

  requireVectorsEqual(ray.origin(), Vector(1, 2, 3));
}

// The camera faces +Z, wherever it is.
TEST_CASE("Ray through center always points in +Z direction", "[camera]") {
  Camera first(Vector(0, 0, -1), 2);
  Camera second(Vector(5, -3, 2), 2);

  requireVectorsEqual(first.rayThroughCenter().direction(), Vector(0, 0, 1));
  requireVectorsEqual(second.rayThroughCenter().direction(), Vector(0, 0, 1));
}

// The screen is a flat rectangle in the plane z = 0, centred on (0, 0, 0), and the direction of a
// ray is (point of the screen) - (camera position).
//
// In a 3 x 3 image, the pixel (1, 1) is the middle one: its centre is the centre of the screen.
//   x = ((1 + 0.5) - 3 / 2) * pixelSize = 0      y = -((1 + 0.5) - 3 / 2) * pixelSize = 0
TEST_CASE("Ray through the middle pixel goes through the center of the screen", "[camera]") {
  Camera camera(Vector(0, 0, -1), 3);

  Ray ray = camera.rayThroughPixel(1, 1, 3, 3);

  requireVectorsEqual(ray.origin(), Vector(0, 0, -1));
  requireVectorsEqual(ray.direction(), Vector(0, 0, 1));
}

// A 4 x 2 image on a screen 4 wide: a pixel is 4 / 4 = 1 unit wide.
//   pixel (0, 0): x = (0.5 - 2) * 1 = -1.5, row 0 is above the middle: y = -(0.5 - 1) * 1 = +0.5
//   pixel (3, 1): x = (3.5 - 2) * 1 = +1.5, row 1 is below the middle: y = -(1.5 - 1) * 1 = -0.5
// The camera is at (0, 0, -1) and the screen at z = 0, so z of the direction is 0 - (-1) = 1.
TEST_CASE("Ray through the corner pixels looks at the corners of the screen", "[camera]") {
  Camera camera(Vector(0, 0, -1), 4);

  Ray topLeft = camera.rayThroughPixel(0, 0, 4, 2);
  Ray bottomRight = camera.rayThroughPixel(3, 1, 4, 2);

  requireVectorsEqual(topLeft.origin(), Vector(0, 0, -1));
  requireVectorsEqual(topLeft.direction(), Vector(-1.5, 0.5, 1));
  requireVectorsEqual(bottomRight.origin(), Vector(0, 0, -1));
  requireVectorsEqual(bottomRight.direction(), Vector(1.5, -0.5, 1));
}

// The same image on a screen 2 wide: a pixel is 2 / 4 = 0.5 unit wide, so the top left pixel
// is at x = (0.5 - 2) * 0.5 = -0.75 and y = -(0.5 - 1) * 0.5 = 0.25.
// (If only the half width were scaled, x would be 0.5 - 2 * 0.5 = -0.5.)
TEST_CASE("Ray through a pixel scales with the screen width", "[camera]") {
  Camera camera(Vector(0, 0, -1), 2);

  Ray ray = camera.rayThroughPixel(0, 0, 4, 2);

  requireVectorsEqual(ray.direction(), Vector(-0.75, 0.25, 1));
}

// The screen stays where it is when the camera moves. A camera at (2, 0, -4) looks at the pixel
// (1, 1) of a 3 x 3 image, the centre of the screen (0, 0, 0): direction (0, 0, 0) - (2, 0, -4).
// The pixel (0, 1) is one pixel to the left (pixelSize = 1): the point (-1, 0, 0), so the
// direction is (-1, 0, 0) - (2, 0, -4).
TEST_CASE("Ray through a pixel goes from the camera to the point of the screen", "[camera]") {
  Camera camera(Vector(2, 0, -4), 3);

  Ray middle = camera.rayThroughPixel(1, 1, 3, 3);
  Ray left = camera.rayThroughPixel(0, 1, 3, 3);

  requireVectorsEqual(middle.origin(), Vector(2, 0, -4));
  requireVectorsEqual(middle.direction(), Vector(-2, 0, 4));
  requireVectorsEqual(left.direction(), Vector(-3, 0, 4));
}

// A 5 x 3 image on a screen 5 wide: a pixel is 5 / 5 = 1 unit wide. The camera is at (0, 0, -1)
// and the screen at z = 0, so z of every direction is 0 - (-1) = 1.
//
// The pixel (2, 1) is in the middle of the image:
//   x = ((2 + 0.5) - 5 / 2) * 1 = 0      y = -((1 + 0.5) - 3 / 2) * 1 = 0
TEST_CASE("Ray through pixel (2, 1) of a 5 x 3 image points at the center", "[camera]") {
  Camera camera(Vector(0, 0, -1), 5);

  Ray ray = camera.rayThroughPixel(2, 1, 5, 3);

  requireVectorsEqual(ray.origin(), Vector(0, 0, -1));
  requireVectorsEqual(ray.direction(), Vector(0, 0, 1));
}

// The pixel (4, 0) is at the top right:
//   x = ((4 + 0.5) - 5 / 2) * 1 = 2      y = -((0 + 0.5) - 3 / 2) * 1 = 1
TEST_CASE("Ray through pixel (4, 0) of a 5 x 3 image points at the top right", "[camera]") {
  Camera camera(Vector(0, 0, -1), 5);

  Ray ray = camera.rayThroughPixel(4, 0, 5, 3);

  requireVectorsEqual(ray.origin(), Vector(0, 0, -1));
  requireVectorsEqual(ray.direction(), Vector(2, 1, 1));
}

// The pixel (0, 2) is at the bottom left:
//   x = ((0 + 0.5) - 5 / 2) * 1 = -2     y = -((2 + 0.5) - 3 / 2) * 1 = -1
TEST_CASE("Ray through pixel (0, 2) of a 5 x 3 image points at the bottom left", "[camera]") {
  Camera camera(Vector(0, 0, -1), 5);

  Ray ray = camera.rayThroughPixel(0, 2, 5, 3);

  requireVectorsEqual(ray.origin(), Vector(0, 0, -1));
  requireVectorsEqual(ray.direction(), Vector(-2, -1, 1));
}
// A 512 x 256 image on a screen 2 wide: a pixel is 2 / 512 = 1 / 256 = 0.00390625 unit wide.
// The image is twice as wide as it is high, and the screen is 2 x 1. The camera is at (0, 0, -1),
// so z of every direction is 1.
//
// The pixel (0, 0) is at the top left:
//   x = (0.5 - 512 / 2) / 256 = -255.5 / 256 = -0.998046875
//   y = -(0.5 - 256 / 2) / 256 = 127.5 / 256 = 0.498046875
// The pixel (511, 255) is at the bottom right:
//   x = (511.5 - 256) / 256 = 255.5 / 256      y = -(255.5 - 128) / 256 = -127.5 / 256
// The image has an even size, so no pixel is exactly in the middle: the pixel (256, 128), just
// right of and under the centre, is half a pixel away from it on each axis:
//   x = 0.5 / 256 = 0.001953125                y = -0.5 / 256 = -0.001953125
TEST_CASE("Ray through pixels of a 512 x 256 image on a screen 2 wide", "[camera]") {
  Camera camera(Vector(0, 0, -1), 2);

  Ray topLeft = camera.rayThroughPixel(0, 0, 512, 256);
  Ray bottomRight = camera.rayThroughPixel(511, 255, 512, 256);
  Ray nearCenter = camera.rayThroughPixel(256, 128, 512, 256);

  requireVectorsEqual(topLeft.origin(), Vector(0, 0, -1));
  requireVectorsEqual(topLeft.direction(), Vector(-255.5 / 256, 127.5 / 256, 1));
  requireVectorsEqual(bottomRight.direction(), Vector(255.5 / 256, -127.5 / 256, 1));
  requireVectorsEqual(nearCenter.direction(), Vector(0.5 / 256, -0.5 / 256, 1));
}

// render() sends a ray through every pixel: the pixel is red if the ray hits the sphere, black if
// it does not. The camera is at (0, 0, -1) and the screen is 3 wide, so in a 3 x 3 image a pixel is
// 1 unit wide. The sphere at (0, 0, 1) with a radius of 1 is seen at an angle of 30 degrees:
//   - the middle pixel looks straight at its centre: hit;
//   - the pixel (1, 0) looks along (1, 0, 1) from (0, 0, -1), and passes at 1.41 from the centre
//     of the sphere, more than the radius: miss. So do all the others.
TEST_CASE("Camera render paints only the middle pixel for a small sphere", "[camera]") {
  Camera camera(Vector(0, 0, -1), 3);
  Sphere sphere = flatSphere(Vector(0, 0, 1), 1, Colour(1, 0, 0));
  Image image(3, 3);

  camera.render(sphere, whiteLight(), image);

  for (unsigned int row = 0; row < 3; ++row) {
    for (unsigned int column = 0; column < 3; ++column) {
      bool isMiddle = column == 1 && row == 1;
      requireColoursEqual(image.pixel(column, row), isMiddle ? Colour(1, 0, 0) : Colour(0, 0, 0));
    }
  }
}

// The sphere of main: at (0, 0, 9) with a radius of 5, seen from (0, 0, -1). A pixel is 0.4 wide
// in a 5 x 5 image on a screen 2 wide. The sphere covers the part of the screen that is closer
// than tan(30 degrees) = 0.577 to the centre. The 3 x 3 pixels in the middle are inside it (the
// farthest, at (0.4, 0.4), is at 0.566), and the pixels of the border are outside (at 0.8).
TEST_CASE("Camera render paints the pixels in front of the sphere", "[camera]") {
  Camera camera(Vector(0, 0, -1), 2);
  Sphere sphere = flatSphere(Vector(0, 0, 9), 5, Colour(1, 0, 0));
  Image image(5, 5);

  camera.render(sphere, whiteLight(), image);

  for (unsigned int row = 0; row < 5; ++row) {
    for (unsigned int column = 0; column < 5; ++column) {
      bool isInside = column >= 1 && column <= 3 && row >= 1 && row <= 3;
      requireColoursEqual(image.pixel(column, row), isInside ? Colour(1, 0, 0) : Colour(0, 0, 0));
    }
  }
}

// Every pixel is written, also the ones that miss: render does not rely on the image being black.
TEST_CASE("Camera render paints the missed pixels black", "[camera]") {
  Camera camera(Vector(0, 0, -1), 3);
  Sphere sphere = flatSphere(Vector(0, 0, 1), 1, Colour(1, 0, 0));
  Image image(3, 3);
  image.setPixel(0, 0, Colour(0, 1, 0));

  camera.render(sphere, whiteLight(), image);

  requireColoursEqual(image.pixel(0, 0), Colour(0, 0, 0));
}

// The pixels that show the sphere get the colour given by the material of the sphere: here a
// yellow FlatShader.
TEST_CASE("Camera render uses the material of the sphere", "[camera]") {
  Camera camera(Vector(0, 0, -1), 3);
  Sphere sphere = flatSphere(Vector(0, 0, 1), 1, Colour(1, 1, 0));
  Image image(3, 3);

  camera.render(sphere, whiteLight(), image);

  requireColoursEqual(image.pixel(1, 1), Colour(1, 1, 0));
}

// The camera at (0, 0, -1) looks at the middle of the sphere of centre (0, 0, 1) and radius 1
// through the middle pixel of a 3 x 3 image. The ray (0, 0, -1) + t * (0, 0, 1) hits the sphere at
// (0, 0, 0), where the normal is (0, 0, -1): the surface faces the camera.
//
// The light at (3, 0, -4) is in front of the sphere: its direction from the hit point is
// (3, 0, -4) / 5 = (0.6, 0, -0.8), and the dot product with the normal is 0.8.
TEST_CASE("Camera render gives the light to the material of the sphere", "[camera]") {
  Camera camera(Vector(0, 0, -1), 3);
  Sphere sphere(Vector(0, 0, 1), 1, std::make_shared<LambertShader>(Colour(1, 0.5, 0.25)));
  Light light(Vector(3, 0, -4), Colour(1, 1, 1));
  Image image(3, 3);

  camera.render(sphere, light, image);

  requireColoursEqual(image.pixel(1, 1), Colour(0.8, 0.4, 0.2));
}

// The same sphere with the light behind it, at (0, 0, 5): the side that faces the camera gets no
// light, so the pixel is black although the sphere is hit.
TEST_CASE("Camera render leaves the side away from the light dark", "[camera]") {
  Camera camera(Vector(0, 0, -1), 3);
  Sphere sphere(Vector(0, 0, 1), 1, std::make_shared<LambertShader>(Colour(1, 1, 0)));
  Light light(Vector(0, 0, 5), Colour(1, 1, 1));
  Image image(3, 3);
  image.setPixel(1, 1, Colour(0, 1, 0));

  camera.render(sphere, light, image);

  requireColoursEqual(image.pixel(1, 1), Colour(0, 0, 0));
}

// The top right pixel of a 512 x 256 image on a screen 2 wide: a pixel is 2 / 512 = 1 / 256 wide.
//   x = (511.5 - 256) / 256 = 511 / 512      y = -(0.5 - 128) / 256 = 255 / 512
TEST_CASE("Ray through the top right pixel of a 512 x 256 image", "[camera]") {
  Camera camera(Vector(0, 0, -1), 2);

  Ray ray = camera.rayThroughPixel(511, 0, 512, 256);

  requireVectorsEqual(ray.direction(), Vector(511.0 / 512.0, 255.0 / 512.0, 1));
}

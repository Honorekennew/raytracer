#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "raytracer/geometry/ray.hpp"
#include "raytracer/math/vector.hpp"
#include "raytracer/render/camera.hpp"
#include "test_helpers.hpp"

using Catch::Matchers::WithinAbs;
using raytracer::Camera;
using raytracer::Ray;
using raytracer::Vector;

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

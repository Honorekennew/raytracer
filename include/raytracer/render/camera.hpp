#pragma once

#include "raytracer/geometry/ray.hpp"
#include "raytracer/math/vector.hpp"

namespace raytracer {

// A camera: the point from which the scene is seen.
// For now it always faces the positive Z-axis, so only its position and the width of its screen
// are stored.
//
// The screen is a flat rectangle in the plane z = 0, centred on the point (0, 0, 0). It does not
// move with the camera. A ray goes from the camera through a point of the screen, so the camera
// must be in front of the screen, at a negative z, to look at it.
class Camera {
 public:
  // Creates a camera at "position", whose screen is "screenWidth" units wide.
  // A wider screen means a wider view: with the camera at z = -1 and a screen width of 2, the
  // view is 90 degrees wide.
  Camera(const Vector& position, double screenWidth);

  // Read-only access to the position and the screen width.
  const Vector& position() const;
  double screenWidth() const;

  // Returns the ray that starts at the camera and goes straight ahead (towards +Z).
  Ray rayThroughCenter() const;

  // Returns the ray that starts at the camera and goes through the centre of the pixel at
  // "column" and "row", in an image that is "imageWidth" pixels wide and "imageHeight" high.
  //
  // The image is laid over the whole screen: its width covers "screenWidth" units, and its
  // pixels are squares, so its height covers imageHeight * screenWidth / imageWidth units.
  // Like in Image, the pixel (0, 0) is at the top left, so a pixel in the top row looks up
  // (positive y) and a pixel in the left column looks left (negative x).
  //
  // The direction is (point of the screen) - (camera position). It is not normalised.
  // "imageWidth" and "imageHeight" must not be 0, and the pixel must be inside the image:
  // this is not checked yet.
  Ray rayThroughPixel(unsigned int column, unsigned int row, unsigned int imageWidth,
                      unsigned int imageHeight) const;

 private:
  Vector _position;
  double _screenWidth;
};

}  // namespace raytracer

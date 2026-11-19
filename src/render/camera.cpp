#include "raytracer/render/camera.hpp"

#include <optional>

#include "raytracer/material/material.hpp"

namespace raytracer {

Camera::Camera(const Vector& position, double screenWidth)
    : _position(position), _screenWidth(screenWidth) {}

const Vector& Camera::position() const { return _position; }

double Camera::screenWidth() const { return _screenWidth; }

Ray Camera::rayThroughCenter() const { return Ray(_position, Vector(0, 0, 1)); }

Ray Camera::rayThroughPixel(unsigned int column, unsigned int row, unsigned int imageWidth,
                            unsigned int imageHeight) const {
  // The image covers the whole screen, so one pixel is screenWidth / imageWidth units wide.
  double pixelSize = _screenWidth / imageWidth;

  // Position of the middle of this pixel on the screen. We first count in pixels from the middle
  // of the image, then multiply by the size of a pixel. The "+ 0.5" aims at the middle of the
  // pixel and not at its top left corner. The brackets matter: the whole distance in pixels is
  // scaled, not only the half width.
  // Rows count downwards in the image, but y points up in the scene: we flip its sign.
  double x = ((column + 0.5) - imageWidth / 2.0) * pixelSize;
  double y = -((row + 0.5) - imageHeight / 2.0) * pixelSize;

  // The screen is in the plane z = 0.
  Vector pixelPosition(x, y, 0);
  Vector direction = pixelPosition - _position;
  return Ray(_position, direction);
}

Image Camera::render(const Scene& scene, unsigned int imageWidth, unsigned int imageHeight) const {
  Image image(imageWidth, imageHeight);
  for (unsigned int row = 0; row < imageHeight; ++row) {
    for (unsigned int column = 0; column < imageWidth; ++column) {
      Ray ray = rayThroughPixel(column, row, imageWidth, imageHeight);
      image.setPixel(column, row, trace(ray, scene));
    }
  }
  return image;
}

Colour Camera::trace(const Ray& ray, const Scene& scene) const {
  std::optional<Hit> hit = scene.nearestHit(ray);
  if (!hit.has_value()) {
    return Colour(0, 0, 0);
  }

  return hit->material().shade(ray, *hit, scene.toLight());
}

}  // namespace raytracer

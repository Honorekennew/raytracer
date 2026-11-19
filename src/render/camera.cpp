#include "raytracer/render/camera.hpp"

#include <optional>

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

void Camera::render(const Sphere& sphere, const Light& light, Image& image) const {
  for (unsigned int row = 0; row < image.height(); ++row) {
    for (unsigned int column = 0; column < image.width(); ++column) {
      Ray ray = rayThroughPixel(column, row, image.width(), image.height());
      image.setPixel(column, row, trace(ray, sphere, light));
    }
  }
}

Colour Camera::trace(const Ray& ray, const Sphere& sphere, const Light& light) const {
  std::optional<HitInfo> hit = sphere.hit(ray);
  if (!hit.has_value()) {
    return Colour(0, 0, 0);
  }
  return sphere.material().shade(ray, *hit, light);
}

}  // namespace raytracer

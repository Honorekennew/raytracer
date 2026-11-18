#include "raytracer/render/camera.hpp"

namespace raytracer {

Camera::Camera(const Vector& position) : _position(position) {}

const Vector& Camera::position() const { return _position; }

Ray Camera::rayThroughCenter() const { return Ray(_position, Vector(0, 0, 1)); }

}  // namespace raytracer

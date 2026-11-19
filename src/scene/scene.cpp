#include "raytracer/scene/scene.hpp"

#include <utility>

namespace raytracer {

namespace {

// The direction with length 1. A vector of length 0 has no direction and can't be divided by its
// length: it is kept as it is.
Vector unitOrZero(const Vector& direction) {
  if (direction.length() == 0.0) {
    return direction;
  }
  return direction.normalised();
}

}  // namespace

Scene::Scene(const Vector& toLight) : _toLight(unitOrZero(toLight)) {}

const Vector& Scene::toLight() const { return _toLight; }

// The scene becomes the owner of the shape: "std::move" hands the pointer over, so the caller no
// longer has it.
void Scene::add(std::unique_ptr<Shape> shape) { _shapes.push_back(std::move(shape)); }

// Asks every shape for its hit, and keeps the one with the smallest t: the closest to the origin
// of the ray. Going through all the shapes, in any order, gives the same answer.
std::optional<Hit> Scene::nearestHit(const Ray& ray) const {
  std::optional<Hit> nearest;

  for (const std::unique_ptr<Shape>& shape : _shapes) {
    std::optional<Hit> hit = shape->hit(ray);
    if (hit.has_value() && (!nearest.has_value() || hit->t() < nearest->t())) {
      nearest = hit;
    }
  }

  return nearest;
}

}  // namespace raytracer

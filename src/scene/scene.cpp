#include "raytracer/scene/scene.hpp"

#include <utility>

namespace raytracer {

Scene::Scene(const Vector& toLight) : _toLight(toLight) {}

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

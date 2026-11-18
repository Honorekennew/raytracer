#include "raytracer/render/image.hpp"

namespace raytracer {

// "_pixels(width * height, Colour(0, 0, 0))" makes an array with one black Colour per pixel.
Image::Image(unsigned int width, unsigned int height)
    : _width(width), _height(height), _pixels(width * height, Colour(0.0, 0.0, 0.0)) {}

unsigned int Image::width() const { return _width; }

unsigned int Image::height() const { return _height; }

void Image::setPixel(unsigned int x, unsigned int y, const Colour& colour) {
  _pixels[y * _width + x] = colour;
}

const Colour& Image::pixel(unsigned int x, unsigned int y) const { return _pixels[y * _width + x]; }

}  // namespace raytracer

#include "framebuffer.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace raster {

Framebuffer::Framebuffer(int width, int height) : width_(width), height_(height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Framebuffer: width and height must be positive");
    }
    // One black pixel per cell
    pixels_.assign(static_cast<std::size_t>(width) * static_cast<std::size_t>(height),
                   Color{0.0f, 0.0f, 0.0f});
}

int Framebuffer::width() const {
    return width_;
}

int Framebuffer::height() const {
    return height_;
}

void Framebuffer::clear(Color c) {
    std::fill(pixels_.begin(), pixels_.end(), c);
}

void Framebuffer::set_pixel(int x, int y, Color c) {
    pixels_[index(x, y)] = c;
}

Color Framebuffer::pixel(int x, int y) const {
    return pixels_[index(x, y)];
}

std::size_t Framebuffer::index(int x, int y) const {
    // Out-of-bounds is a caller bug
    assert(x >= 0 && x < width_ && y >= 0 && y < height_);
    // i = y * W + x
    return static_cast<std::size_t>(y * width_ + x);
}

}  // namespace raster

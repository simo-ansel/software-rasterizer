#pragma once

#include <cstddef>
#include <vector>

#include "color.hpp"

namespace raster {

class Framebuffer {
   public:
    // Throws std::invalid_argument if width or height is not positive
    Framebuffer(int width, int height);
    int width() const;
    int height() const;
    // Fill every pixel with one color
    void clear(Color c);
    void set_pixel(int x, int y, Color c);
    Color pixel(int x, int y) const;

   private:
    // Pixel index in memory
    std::size_t index(int x, int y) const;

    int width_;
    int height_;
    // Rows stored one after another (row-major)
    std::vector<Color> pixels_;
};

}  // namespace raster

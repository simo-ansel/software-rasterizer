#include "scenes.hpp"

namespace raster {

Framebuffer render_gradient(int width, int height) {
    Framebuffer fb(width, height);
    // y outside, x inside: memory is read in order
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Pixel center in [0, 1]
            const float u = (static_cast<float>(x) + 0.5f) / static_cast<float>(width);
            const float v = (static_cast<float>(y) + 0.5f) / static_cast<float>(height);
            fb.set_pixel(x, y, Color{u, v, 0.25f});
        }
    }
    return fb;
}

}  // namespace raster

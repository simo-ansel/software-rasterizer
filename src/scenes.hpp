#pragma once

#include "framebuffer.hpp"

namespace raster {

// Test image: red grows with x, green with y
Framebuffer render_gradient(int width, int height);

}  // namespace raster

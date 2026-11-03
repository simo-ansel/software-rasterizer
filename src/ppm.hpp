#pragma once

#include <iosfwd>

#include "framebuffer.hpp"

namespace raster {

// Writes a binary PPM (P6) to the stream
void write_ppm(std::ostream& out, const Framebuffer& fb);

}  // namespace raster

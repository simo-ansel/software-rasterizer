#pragma once

#include <cstdint>

namespace raster {

// Linear RGB, each channel in [0, 1]
struct Color {
    float r, g, b;
};

// Float channel -> byte in [0, 255]
std::uint8_t to_byte(float c);

}  // namespace raster

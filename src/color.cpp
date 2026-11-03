#include "color.hpp"

#include <algorithm>
#include <cmath>

namespace raster {

std::uint8_t to_byte(float c) {
    // NaN has no sensible color: black
    if (std::isnan(c)) {
        return 0;
    }
    // Out-of-range values saturate
    const float limited = std::clamp(c, 0.0f, 1.0f);
    // +0.5 rounds to nearest, the cast truncates
    return static_cast<std::uint8_t>(limited * 255.0f + 0.5f);
}

}  // namespace raster

#include "ppm.hpp"

#include <ostream>

namespace raster {

void write_ppm(std::ostream& out, const Framebuffer& fb) {
    // Header: type, size, max sample value
    out << "P6\n" << fb.width() << ' ' << fb.height() << "\n255\n";
    // Rows from top to bottom
    for (int y = 0; y < fb.height(); ++y) {
        for (int x = 0; x < fb.width(); ++x) {
            const Color c = fb.pixel(x, y);
            // R G B bytes of one pixel
            const char rgb[3] = {static_cast<char>(to_byte(c.r)), static_cast<char>(to_byte(c.g)),
                                 static_cast<char>(to_byte(c.b))};
            out.write(rgb, 3);
        }
    }
}

}  // namespace raster

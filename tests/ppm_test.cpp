#include "ppm.hpp"

#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <vector>

using raster::Color;
using raster::Framebuffer;

TEST(WritePpm, ThreeByTwoImageIsExact) {
    Framebuffer fb(3, 2);
    fb.set_pixel(0, 0, Color{1.0f, 0.0f, 0.0f});  // red
    fb.set_pixel(1, 0, Color{0.0f, 1.0f, 0.0f});  // green
    fb.set_pixel(2, 0, Color{0.0f, 0.0f, 1.0f});  // blue
    fb.set_pixel(0, 1, Color{1.0f, 1.0f, 1.0f});  // white
    fb.set_pixel(1, 1, Color{0.0f, 0.0f, 0.0f});  // black
    fb.set_pixel(2, 1, Color{0.5f, 0.5f, 0.5f});  // gray

    std::ostringstream out;
    raster::write_ppm(out, fb);

    // Header is 11 bytes, pixels are 18: 29 in total
    std::string expected = "P6\n3 2\n255\n";
    const std::vector<unsigned char> pixels = {
        0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,  // first row
        0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80,  // second row
    };
    expected.append(pixels.begin(), pixels.end());

    EXPECT_EQ(out.str().size(), 29u);
    EXPECT_EQ(out.str(), expected);
}

TEST(WritePpm, HeaderUsesWidthThenHeight) {
    const Framebuffer fb(5, 2);
    std::ostringstream out;
    raster::write_ppm(out, fb);
    EXPECT_EQ(out.str().substr(0, 11), "P6\n5 2\n255\n");
    // 11 header bytes + 3 bytes per pixel
    EXPECT_EQ(out.str().size(), 11u + 3u * 5u * 2u);
}

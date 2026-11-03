#include "scenes.hpp"

#include <gtest/gtest.h>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "ppm.hpp"

using raster::Framebuffer;

namespace {

constexpr int kWidth = 320;
constexpr int kHeight = 180;
constexpr std::size_t kHeaderBytes = 15;  // "P6\n320 180\n255\n"

std::string gradient_file() {
    std::ostringstream out;
    raster::write_ppm(out, raster::render_gradient(kWidth, kHeight));
    return out.str();
}

// R, G, B bytes of pixel (x, y) in the file, found with offset = 15 + 3 * (y * W + x)
std::string bytes_at(const std::string& file, int x, int y) {
    const std::size_t offset = kHeaderBytes + 3 * static_cast<std::size_t>(y * kWidth + x);
    return file.substr(offset, 3);
}

std::string hex(const std::string& bytes) {
    static const char* digits = "0123456789abcdef";
    std::string result;
    for (char c : bytes) {
        const auto value = static_cast<unsigned char>(c);
        result += digits[value >> 4];
        result += digits[value & 0x0F];
        result += ' ';
    }
    return result;
}

}  // namespace

TEST(Gradient, SamplesThePixelCenter) {
    // The 16x9 version used in the notebook: pixel (5, 2) -> bytes 88 71 64
    const Framebuffer fb = raster::render_gradient(16, 9);
    const raster::Color c = fb.pixel(5, 2);
    EXPECT_NEAR(c.r, 5.5f / 16.0f, 1e-6f);
    EXPECT_NEAR(c.g, 2.5f / 9.0f, 1e-6f);
    EXPECT_NEAR(c.b, 0.25f, 1e-6f);
    EXPECT_EQ(static_cast<int>(raster::to_byte(c.r)), 88);
    EXPECT_EQ(static_cast<int>(raster::to_byte(c.g)), 71);
    EXPECT_EQ(static_cast<int>(raster::to_byte(c.b)), 64);
}

TEST(Gradient, FileHasTheExpectedSize) {
    // 15 header bytes + 3 * 320 * 180
    EXPECT_EQ(gradient_file().size(), 172815u);
}

TEST(Gradient, FourCornersHaveTheExpectedBytes) {
    const std::string file = gradient_file();
    EXPECT_EQ(hex(bytes_at(file, 0, 0)), "00 01 40 ");
    EXPECT_EQ(hex(bytes_at(file, 319, 0)), "ff 01 40 ");
    EXPECT_EQ(hex(bytes_at(file, 0, 179)), "00 fe 40 ");
    EXPECT_EQ(hex(bytes_at(file, 319, 179)), "ff fe 40 ");
}

TEST(Gradient, MatchesTheReferenceImage) {
    const std::string path = std::string(RASTER_REFERENCE_DIR) + "/t1_gradient.ppm";
    std::ifstream in(path, std::ios::binary);
    ASSERT_TRUE(in) << "cannot open " << path;
    std::ostringstream buffer;
    buffer << in.rdbuf();
    const std::string reference = buffer.str();

    const std::string actual = gradient_file();
    ASSERT_EQ(actual.size(), reference.size());
    // Tolerance of one level per channel
    for (std::size_t i = 0; i < actual.size(); ++i) {
        const int a = static_cast<unsigned char>(actual[i]);
        const int r = static_cast<unsigned char>(reference[i]);
        ASSERT_LE(std::abs(a - r), 1) << "byte " << i;
    }
}

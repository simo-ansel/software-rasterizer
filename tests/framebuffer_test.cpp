#include "framebuffer.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

using raster::Color;
using raster::Framebuffer;

namespace {

void expect_color(Color actual, Color expected) {
    EXPECT_FLOAT_EQ(actual.r, expected.r);
    EXPECT_FLOAT_EQ(actual.g, expected.g);
    EXPECT_FLOAT_EQ(actual.b, expected.b);
}

const Color kRed{1.0f, 0.0f, 0.0f};
const Color kGreen{0.0f, 1.0f, 0.0f};
const Color kBlack{0.0f, 0.0f, 0.0f};

}  // namespace

TEST(Framebuffer, SizeIsNotSwapped) {
    // 3x2 is not square: swapping width and height would fail here
    const Framebuffer fb(3, 2);
    EXPECT_EQ(fb.width(), 3);
    EXPECT_EQ(fb.height(), 2);
}

TEST(Framebuffer, StartsBlack) {
    const Framebuffer fb(3, 2);
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 3; ++x) {
            expect_color(fb.pixel(x, y), kBlack);
        }
    }
}

TEST(Framebuffer, ClearFillsEveryPixel) {
    Framebuffer fb(3, 2);
    fb.clear(kRed);
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 3; ++x) {
            expect_color(fb.pixel(x, y), kRed);
        }
    }
}

TEST(Framebuffer, SetThenGet) {
    Framebuffer fb(3, 2);
    const Color c{0.25f, 0.5f, 0.75f};
    fb.set_pixel(1, 1, c);
    expect_color(fb.pixel(1, 1), c);
    expect_color(fb.pixel(0, 0), kBlack);
}

TEST(Framebuffer, NeighboursInMemoryStayDistinct) {
    // (2, 0) and (0, 1) sit next to each other in memory but are different pixels
    Framebuffer fb(3, 2);
    fb.set_pixel(2, 0, kRed);
    fb.set_pixel(0, 1, kGreen);
    expect_color(fb.pixel(2, 0), kRed);
    expect_color(fb.pixel(0, 1), kGreen);
    expect_color(fb.pixel(1, 0), kBlack);
    expect_color(fb.pixel(1, 1), kBlack);
}

TEST(Framebuffer, InvalidSizeThrows) {
    EXPECT_THROW(Framebuffer(0, 2), std::invalid_argument);
    EXPECT_THROW(Framebuffer(3, 0), std::invalid_argument);
    EXPECT_THROW(Framebuffer(-1, 2), std::invalid_argument);
    EXPECT_THROW(Framebuffer(3, -2), std::invalid_argument);
}

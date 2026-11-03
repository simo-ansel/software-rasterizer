#include "color.hpp"

#include <gtest/gtest.h>

#include <limits>

namespace {

// Plain int, so a failure prints a number and not a character
int byte(float c) {
    return raster::to_byte(c);
}

}  // namespace

TEST(ToByte, KnownValues) {
    EXPECT_EQ(byte(0.0f), 0);
    EXPECT_EQ(byte(0.25f), 64);
    EXPECT_EQ(byte(0.5f), 128);
    EXPECT_EQ(byte(0.75f), 191);
    EXPECT_EQ(byte(1.0f), 255);
}

TEST(ToByte, OutOfRangeSaturates) {
    EXPECT_EQ(byte(-0.2f), 0);
    EXPECT_EQ(byte(1.5f), 255);
    EXPECT_EQ(byte(-std::numeric_limits<float>::infinity()), 0);
    EXPECT_EQ(byte(std::numeric_limits<float>::infinity()), 255);
}

TEST(ToByte, NaNIsBlack) {
    EXPECT_EQ(byte(std::numeric_limits<float>::quiet_NaN()), 0);
}

TEST(ToByte, NeverDecreases) {
    // Every step of 1/1000 must give the same byte or a larger one
    int previous = byte(0.0f);
    for (int i = 1; i <= 1000; ++i) {
        const int current = byte(static_cast<float>(i) / 1000.0f);
        EXPECT_GE(current, previous) << "at i = " << i;
        previous = current;
    }
}

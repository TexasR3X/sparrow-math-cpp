#include <gtest/gtest.h>
#include "sparrow_math/sparrow_math.hpp"

TEST(AddTest, HandlesPositiveNumbers) {
    EXPECT_EQ(sparrow_math::add(2, 3), 5);
}

TEST(AddTest, HandlesNegativeNumbers1) {
    EXPECT_EQ(sparrow_math::add(-2, -3), -5);
}

TEST(AddTest, HandlesNegativeNumbers2) {
    EXPECT_EQ(sparrow_math::add(2, -3), -1);
}

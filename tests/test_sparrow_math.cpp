#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "sparrow_math/sparrow_math.hpp"
#include "internal/ParsingUtils/TokenUtils.hpp"

namespace ParsingUtils = sparrow_math::internal::ParsingUtils;

TEST(AddTest, HandlesPositiveNumbers) {
    EXPECT_EQ(sparrow_math::add(2, 3), 5);
}

TEST(AddTest, HandlesNegativeNumbers1) {
    EXPECT_EQ(sparrow_math::add(-2, -3), -5);
}

TEST(AddTest, HandlesNegativeNumbers2) {
    EXPECT_EQ(sparrow_math::add(2, -3), -1);
}

TEST(TokenUtils, Test1) {
    // std::string input = "22 + (2 * pi  + 8^(6 - 867*x)    )  ";
    std::string input = "23 - 94";
    
    std::cout << "INPUT: " << input << std::endl;

    std::vector<ParsingUtils::Token> actual = ParsingUtils::TokenizeLatex(input);

    std::cout << "actual.size(): " << actual.size() << std::endl;

    std::cout << "ACTUAL TOKENS:" << std::endl;

    for (auto token : actual) {
        std::cout << token.ToString() << std::endl;
    }
}

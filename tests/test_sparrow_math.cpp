#include <format>
#include <iostream>
#include <vector>
#include <gtest/gtest.h>
#include "sparrow_math/sparrow_math.hpp"
// #include "internal/str_utils.hpp"
// #include "internal/parsing_utils/token_utils.hpp"
#include "internal/parsing_utils/expr_utils.hpp"

namespace parsing_utils = sparrow_math::internal::parsing_utils;

TEST(AddTest, HandlesPositiveNums) {
    EXPECT_EQ(sparrow_math::add(2, 3), 5);
}

TEST(AddTest, HandlesNegativeNums1) {
    EXPECT_EQ(sparrow_math::add(-2, -3), -5);
}

TEST(AddTest, HandlesNegativeNums2) {
    EXPECT_EQ(sparrow_math::add(2, -3), -1);
}

TEST(TokenUtils, Test1) {
    std::cout << "__cplusplus: " << __cplusplus << std::endl;

    char four = '4';

    std::cout << std::format("Test: {}", four) << std::endl;

    // std::string input = R"(4*60*52++9*2 - \\ \+ 9+)";

    // std::cout << "input: " << input << std::endl;

    // auto actual = parsing_utils::TokenizeLatex(input);

    // std::cout << "ACTUAL TOKENS:" << std::endl;

    // for (const auto& token : actual) {
    //     std::cout << token.ToString() << std::endl;
    // }


    

    auto str = parsing_utils::ParseExpr({});

    std::cout << "str: " << str << std::endl;








    // std::string input = "hello there";
    // std::cout << "input: " << input << std::endl;

    // std::string actual = sparrow_math::internal::TrimSymbolName(input);
    // std::cout << "actual: " << actual << std::endl;











    // // std::string input = "22 + (2 * pi  + 8^(6 - 867*x)    )  ";
    // // std::string input = "23           -9+5. -  0 * .";
    // // std::string input = "x - 49y\\pi+";
    // // std::string input = "y^{q-2} 23x^123 + \\\\ 41 \\ 1 \\gamma^2 \\, xy";
    // // std::string input = "\\frac{1}{\\sigma\\sqrt{2\\pi}}e^{-\\frac{(x-\\mu)^2}{2\\sigma^2}}";
    // // std::string input = R"(
    // //     \begin{bmatrix}
    // //         1 & 2 \\
    // //         3 & 4
    // //     \end{bmatrix}
    // // )";
    // std::string input = R"(\int_0^1 x^2\,dx \cdot \sum_{\index = 1}^\infty \frac{1}{\index^2})";
    // // std::string input = R"(x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}) \\ P(A\mid B)=\frac{P(A\cap B)}{P(B)})";
    // // std::string input = R"(\frac{d}{dx}\left(x^3\right)=3x^2)";

    // std::cout << "INPUT: " << input << std::endl;

    // std::vector<parsing_utils::Token> actual = parsing_utils::TokenizeLatex(input);

    // std::cout << "actual.size(): " << actual.size() << std::endl;

    // std::cout << "ACTUAL TOKENS:" << std::endl;

    // for (auto token : actual) {
    //     std::cout << token.ToString() << std::endl;
    // }
}

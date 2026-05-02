#include <stdexcept>
#include <vector>

#pragma once
namespace sparrow_math::internal::ParsingUtils {
    enum class TokenType {
        Number,
        Symbol,
        Plus,
        Minus,
        Star,
        Underscore,
        UpArrow,
        LeftParenthesis,
        RightParenthesis,
        LeftSquareBracket,
        RightSquareBracket,
        LeftCurlyBracket,
        RightCurlyBracket
    };

    struct Token {
        const TokenType Type;
        const std::string Value;

        Token(TokenType type, std::string value = "") : Type(type), Value(value) {}

        std::string ToString() const;
    };

    // DEV NOTE: Create a way to handle errors in the tokenization
    // class TokenizationException : public std::runtime_error {
    //     // TokenizationException(std::string message) : std::runtime_error(message) {}
    // };

    class StringIterator {
    public:
        StringIterator(std::string str) : _str(str), _strLength(str.length()) {}

        char Peak() const;

        char Advance();

        bool IsFinished() const;

    private:
        const std::string _str;
        const size_t _strLength;
        size_t _currentIndex = 0;
    };

    std::vector<Token> TokenizeLatex(std::string latex);
}

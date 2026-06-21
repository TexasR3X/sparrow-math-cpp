#include <string>
#include <vector>

#pragma once
namespace sparrow_math::internal::parsing_utils {
    struct Token {
        enum class TokenType {
            Num,
            Symbol,
            Plus,
            Minus,
            Star,
            ForwardSlash,
            Underscore,
            UpArrow,
            Ampersand,
            EqualSign,
            LineBreak,
            LeftParenthesis,
            RightParenthesis,
            LeftSquareBracket,
            RightSquareBracket,
            LeftCurlyBracket,
            RightCurlyBracket,
            EscapedLeftCurlyBracket,
            EscapedRightCurlyBracket,
            LeftAngleBracket,
            RightAngleBracket,
            Null
        };

        TokenType Type;
        std::string Value;

        Token(TokenType type = TokenType::Null, std::string value = "") : Type(type), Value(value) {}

        std::string DebugToString() const;
    };

    std::vector<Token> TokenizeLatex(std::string_view latex);

    void CleanTokens(std::vector<Token>& tokens);
}

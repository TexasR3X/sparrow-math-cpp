#include <stack>
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

        bool IsTokenLeftDelimiter() {
            return Type == TokenType::LeftParenthesis
                || Type == TokenType::LeftSquareBracket
                || Type == TokenType::LeftCurlyBracket
                || Type == TokenType::EscapedLeftCurlyBracket
                || Type == TokenType::LeftAngleBracket;
        }

        bool IsTokenRightDelimiter() {
            return Type == TokenType::RightParenthesis
                || Type == TokenType::RightSquareBracket
                || Type == TokenType::RightCurlyBracket
                || Type == TokenType::EscapedRightCurlyBracket
                || Type == TokenType::RightAngleBracket;
        }

        bool IsTokenDelimiter() {
            return IsTokenLeftDelimiter() || IsTokenRightDelimiter();
        }

        bool IsTokenOperator() {
            return Type == Token::TokenType::Plus
                || Type == Token::TokenType::Minus
                || Type == Token::TokenType::Star
                || Type == Token::TokenType::ForwardSlash
                || Type == Token::TokenType::Underscore
                || Type == Token::TokenType::UpArrow
                || Type == Token::TokenType::Ampersand;
        }

        std::string DebugToString() const;
    };

    class DelimiterBalanceChecker {
    public:
        void ProcessDelimiter(Token::TokenType tokenType);

        void EnsureDelimiterStackIsEmpty();
    private:
        std::stack<Token::TokenType> _delStack;

        void HandleRightDelimiter(Token::TokenType leftDel);
    };

    std::vector<Token> TokenizeLatex(std::string_view latex);

    void CleanTokens(std::vector<Token>& tokens);
}

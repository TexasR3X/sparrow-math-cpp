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
            DoubleAmpersand,
            Pipe,
            DoublePipe,
            EqualSign,
            DoubleEqualSign,
            ExclamationMark,
            NotEqual,
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

        bool IsTokenLeftDelimiter() const {
            return Type == TokenType::LeftParenthesis
                || Type == TokenType::LeftSquareBracket
                || Type == TokenType::LeftCurlyBracket
                || Type == TokenType::EscapedLeftCurlyBracket
                || Type == TokenType::LeftAngleBracket;
        }

        bool IsTokenRightDelimiter() const {
            return Type == TokenType::RightParenthesis
                || Type == TokenType::RightSquareBracket
                || Type == TokenType::RightCurlyBracket
                || Type == TokenType::EscapedRightCurlyBracket
                || Type == TokenType::RightAngleBracket;
        }

        bool IsTokenDelimiter() const {
            return IsTokenLeftDelimiter() || IsTokenRightDelimiter();
        }

        bool IsTokenBinaryOperator() const {
            return Type == Token::TokenType::Star
                || Type == Token::TokenType::ForwardSlash
                || Type == Token::TokenType::Underscore
                || Type == Token::TokenType::UpArrow
                || Type == Token::TokenType::Ampersand
                || Type == Token::TokenType::DoubleAmpersand
                || Type == Token::TokenType::Pipe
                || Type == Token::TokenType::DoublePipe
                || Type == Token::TokenType::EqualSign
                || Type == Token::TokenType::DoubleEqualSign
                || Type == Token::TokenType::NotEqual;
        }

        std::string DebugToString() const;
    };

    class DelimiterBalanceChecker {
    public:
        void ProcessDelimiter(Token::TokenType tokenType);

        void EnsureDelimiterStackIsEmpty() const;
    private:
        std::stack<Token::TokenType> _delStack;

        void HandleRightDelimiter(Token::TokenType leftDel);
    };

    std::vector<Token> TokenizeLatex(std::string_view latex);

    void CleanTokens(std::vector<Token>& tokens);
}

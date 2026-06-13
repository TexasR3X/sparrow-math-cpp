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

        Token(TokenType type, std::string value = "") : Type(type), Value(value) {}

        std::string DebugToString() const;
    };

    class TokenBuilder {
    public:
        enum class AppendingResult {
            StillWorking,
            ReadyToFinishWithLastChar,
            ReadyToFinishWithoutLastChar,
            ErrorFound
        };

        AppendingResult AppendToToken(char ch);
        Token FinishToken();

        std::string ErrorMessage;

        bool SeeIfBuilderHasUnfinishedToken() const;
    private:
        std::string _tokenContents;
        Token::TokenType _tokenType = Token::TokenType::Null;

        bool _hasDotBeenFound = false;
        bool _hasBackslashBeenFound = false;

        bool _tokenMustBeSymbol = false;
        bool _tokenMustBeNum = false;
        bool _tokenMustBeOperator = false;

        void ClearBuilder();
    };

    std::vector<Token> TokenizeLatex(const std::string& latex);
}

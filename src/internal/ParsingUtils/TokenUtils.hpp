#include <stdexcept>
#include <string>
#include <vector>

#pragma once
namespace sparrow_math::internal::ParsingUtils {
    enum class TokenType {
        Number,
        Symbol,
        Plus,
        Minus,
        Star,
        ForwardSlash,
        Underscore,
        UpArrow,
        Ampersand,
        EqualSign,
        DoubleBackslash,
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
        Unknown
    };

    struct Token {
        TokenType Type;
        std::string Value;

        Token(TokenType type, std::string value = "") : Type(type), Value(value) {}

        std::string ToString() const;
    };

    class TokenBuilder {
    public:
        enum class AppendingResult {
            StillWorking,
            ReadyToFinishWithLastChar,
            ReadyToFinishWithoutLastChar,
            ErrorFound
        };

        AppendingResult AppendToToken(const char& ch);
        Token FinishToken();

        std::string ErrorMessage;

        bool HasUnfinishedToken() const;
    private:
        std::string _tokenContents;
        TokenType _tokenType = TokenType::Unknown;

        bool _hasDotBeenFound = false;
        bool _hasBackslashBeenFound = false;

        bool _tokenMustBeSymbol = false;
        bool _tokenMustBeNum = false;
        bool _tokenMustBeOperator = false;

        void ClearBuilder();
    };

    std::vector<Token> TokenizeLatex(std::string latex);
}

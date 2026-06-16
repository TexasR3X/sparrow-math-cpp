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
            FinishedToken
        };

        AppendingResult AppendToToken(char ch);

        std::vector<Token> GetTokens();

        void DebugPrint();
    private:
        std::vector<Token> _tokens;

        std::string _curTokenContents;

        bool _hasDotBeenFoundOnCurToken = false;
        bool _hasBackslashBeenFoundOnCurToken = false;

        bool _curTokenMustBeSymbol = false;
        bool _curTokenMustBeNum = false;
        bool _curTokenMustBeOperator = false;

        void FinishToken();

        void ClearBuilder();
    };

    std::vector<Token> TokenizeLatex(std::string_view latex);
}

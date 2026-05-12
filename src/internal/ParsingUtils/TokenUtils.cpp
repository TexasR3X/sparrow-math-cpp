#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "TokenUtils.hpp"

#define DEBUG_LOG(code) std::cout << "[" << __LINE__ << "] " << #code << ": " << (code) << std::endl

namespace sparrow_math::internal::ParsingUtils {
    std::string Token::ToString() const {
        std::string type;

        switch (Type) {
            case TokenType::Number:
                type = "Number";
                break;
            case TokenType::Symbol:
                type = "Symbol";
                break;
            case TokenType::Plus:
                type = "Plus";
                break;
            case TokenType::Minus:
                type = "Minus";
                break;
            case TokenType::Star:
                type = "Star";
                break;
            case TokenType::Underscore:
                type = "Underscore";
                break;
            case TokenType::UpArrow:
                type = "UpArrow";
                break;
            case TokenType::Ampersand:
                type = "Ampersand";
                break;
            case TokenType::EqualSign:
                type = "EqualSign";
                break;
            case TokenType::DoubleBackslash:
                type = "DoubleBackslash";
                break;
            case TokenType::LeftParenthesis:
                type = "LeftParenthesis";
                break;
            case TokenType::RightParenthesis:
                type = "RightParenthesis";
                break;
            case TokenType::LeftSquareBracket:
                type = "LeftSquareBracket";
                break;
            case TokenType::RightSquareBracket:
                type = "RightSquareBracket";
                break;
            case TokenType::LeftCurlyBracket:
                type = "LeftCurlyBracket";
                break;
            case TokenType::RightCurlyBracket:
                type = "RightCurlyBracket";
                break;
            case TokenType::EscapedLeftCurlyBracket:
                type = "EscapedLeftCurlyBracket";
                break;
            case TokenType::EscapedRightCurlyBracket:
                type = "EscapedRightCurlyBracket";
                break;
            case TokenType::LeftAngleBracket:
                type = "LeftAngleBracket";
                break;
            case TokenType::RightAngleBracket:
                type = "RightAngleBracket";
                break;
            default:
                throw std::runtime_error("Unknown `TokenType` option");
        }

        return type + "(" + Value + ")";
    }

    TokenBuilder::AppendingResult TokenBuilder::AppendToToken(const char& ch) {
        if (_tokenMustBeNum) {
            if (std::isdigit(ch)) {
                _tokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else if (ch == '.') {
                // See if a decimal point has already been found
                if (_hasDotBeenFound) {
                    ErrorMessage = "A number cannot have two decimal points";
                    return AppendingResult::ErrorFound;
                }
                else {
                    _hasDotBeenFound = true;
                    _tokenContents += ch;
                    return AppendingResult::StillWorking;
                }
            }
            else {
                return AppendingResult::ReadyToFinishWithoutLastChar;
            }
        }
        else if (_tokenMustBeSymbol) {
            if (std::isalnum(ch)) {
                _tokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else {
                return AppendingResult::ReadyToFinishWithoutLastChar;
            }
        }
        else if (_hasBackslashBeenFound) {
            if (std::isalpha(ch)) {
                _tokenMustBeSymbol = true;
                _tokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else if (std::isdigit(ch)) {
                ErrorMessage = "A LaTeX command cannot start with a number";
                return AppendingResult::ErrorFound;
            }
            else {
                switch (ch) {
                    case ' ':
                    case ',':
                    case ':':
                    case ';':
                    case '!':
                    // case '\n':
                        ClearBuilder();
                        return AppendingResult::StillWorking;
                    case '\\':
                    case '{':
                    case '}':
                        _tokenMustBeOperator = true;
                        _tokenContents += ch;

                        return AppendingResult::ReadyToFinishWithLastChar;
                    default:
                        ErrorMessage = std::format("Invalid character ({}) infront of \\", ch);
                        return AppendingResult::ErrorFound;
                }
            }
        }
        else {
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
                if (_tokenContents.empty()) {
                    // Don't append the last space character to the working token
                    return AppendingResult::StillWorking;
                }
                else {
                    // Don't append the last space character, but still return `ReadyToFinishWithLastChar` so the space is skipped
                    return AppendingResult::ReadyToFinishWithLastChar;
                }
            }
            else if (ch == '\\') {
                _hasBackslashBeenFound = true;
                _tokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else if (ch == '.') {
                _tokenMustBeNum = true;
                _hasDotBeenFound = true;
                _tokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else if (std::isalpha(ch)) {
                _tokenMustBeSymbol = true;
                _tokenContents += ch;
                return AppendingResult::ReadyToFinishWithLastChar;
            }
            else if (std::isdigit(ch)) {
                _tokenMustBeNum = true;
                _tokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else {
                switch (ch) {
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '_':
                    case '^':
                    case '&':
                    case '=':
                    case '(':
                    case ')':
                    case '[':
                    case ']':
                    case '{':
                    case '}':
                    case '<':
                    case '>':
                        _tokenMustBeOperator = true;
                        _tokenContents += ch;
                        return AppendingResult::ReadyToFinishWithLastChar;
                    default:
                        ErrorMessage = std::format("Invalid character ({}) in LaTeX", ch);
                        return AppendingResult::ErrorFound;
                }
            }
        }
    }

    Token TokenBuilder::FinishToken() {
        Token token(Token::TokenType::Unknown);

        if (_tokenMustBeSymbol) {
            token.Type = Token::TokenType::Symbol;
            token.Value = _tokenContents;
        }
        else if (_tokenMustBeNum) {
            token.Type = Token::TokenType::Number;
            token.Value = _tokenContents;
        }
        else if (_tokenMustBeOperator) {
            if (_tokenContents == "+") {
                token.Type = Token::TokenType::Plus;
            }
            else if (_tokenContents == "-") {
                token.Type = Token::TokenType::Minus;
            }
            else if (_tokenContents == "*") {
                token.Type = Token::TokenType::Star;
            }
            else if (_tokenContents == "/") {
                token.Type = Token::TokenType::ForwardSlash;
            }
            else if (_tokenContents == "_") {
                token.Type = Token::TokenType::Underscore;
            }
            else if (_tokenContents == "^") {
                token.Type = Token::TokenType::UpArrow;
            }
            else if (_tokenContents == "&") {
                token.Type = Token::TokenType::Ampersand;
            }
            else if (_tokenContents == "=") {
                token.Type = Token::TokenType::EqualSign;
            }
            else if (_tokenContents == "\\\\") {
                token.Type = Token::TokenType::DoubleBackslash;
            }
            else if (_tokenContents == "(") {
                token.Type = Token::TokenType::LeftParenthesis;
            }
            else if (_tokenContents == ")") {
                token.Type = Token::TokenType::RightParenthesis;
            }
            else if (_tokenContents == "[") {
                token.Type = Token::TokenType::LeftSquareBracket;
            }
            else if (_tokenContents == "]") {
                token.Type = Token::TokenType::RightSquareBracket;
            }
            else if (_tokenContents == "{") {
                token.Type = Token::TokenType::LeftCurlyBracket;
            }
            else if (_tokenContents == "}") {
                token.Type = Token::TokenType::RightCurlyBracket;
            }
            else if (_tokenContents == "\\{") {
                token.Type = Token::TokenType::EscapedLeftCurlyBracket;
            }
            else if (_tokenContents == "\\}") {
                token.Type = Token::TokenType::EscapedRightCurlyBracket;
            }
            else if (_tokenContents == "<") {
                token.Type = Token::TokenType::LeftAngleBracket;
            }
            else if (_tokenContents == ">") {
                token.Type = Token::TokenType::RightAngleBracket;
            }
            else {
                throw std::runtime_error("Unknown operator found");
            }
        }
        else {
            throw std::runtime_error("Token not found");
        }

        ClearBuilder();

        return token;
    }

    bool TokenBuilder::HasUnfinishedToken() const {
        return !_tokenContents.empty();
    }

    void TokenBuilder::ClearBuilder() {
        _tokenContents.clear();
        ErrorMessage.clear();
        Token::TokenType _tokenType = Token::TokenType::Unknown;

        _tokenMustBeNum = false;
        _tokenMustBeSymbol = false;
        _hasDotBeenFound = false;
        _hasBackslashBeenFound = false;
    }

    std::vector<Token> TokenizeLatex(std::string latex) {
        std::vector<Token> tokens;
        TokenBuilder builder;

        int loopCount = 0;
        constexpr int maxLoopCount = 2000000;

        for (int i = 0; i < latex.length() && loopCount < maxLoopCount; ++i) {
            char ch = latex.at(i);

            TokenBuilder::AppendingResult result = builder.AppendToToken(ch);

            switch (result) {
                case TokenBuilder::AppendingResult::StillWorking:
                    break;
                case TokenBuilder::AppendingResult::ReadyToFinishWithoutLastChar:
                    --i;
                case TokenBuilder::AppendingResult::ReadyToFinishWithLastChar: {
                    Token token = builder.FinishToken();
                    tokens.emplace_back(token);
                    break;
                }
                case TokenBuilder::AppendingResult::ErrorFound:
                    throw std::runtime_error(builder.ErrorMessage);
                default:
                    throw std::runtime_error("Unknown `AppendingResult` option");
            }

            ++loopCount;
        }

        if (loopCount == maxLoopCount) {
            // Throw an error if an infinite loop occurs
            throw std::runtime_error("Infinite loop occurred during tokenization of the LaTeX");
        }
        else if (builder.HasUnfinishedToken()) {
            // If the `builder` isn't finished, try to finish it
            // Throw an exception if it can't be finished
            Token token = builder.FinishToken();
            tokens.emplace_back(token);
        }

        return tokens;
    }
}

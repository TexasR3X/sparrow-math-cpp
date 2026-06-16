#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "token_utils.hpp"
#include "../str_utils.hpp"

namespace sparrow_math::internal::parsing_utils {
    std::string Token::DebugToString() const {
        std::string type;

        switch (Type) {
            case TokenType::Num:
                type = "Num";
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
            case TokenType::LineBreak:
                type = "LineBreak";
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
            case TokenType::Null:
                type = "Null";
                break;
            default:
                type = "Token[" + std::to_string((int)Type) + "]";
        }

        return type + "(" + Value + ")";
    }

    TokenBuilder::AppendingResult TokenBuilder::AppendToToken(char ch) {
        std::cout << "ch: " << ch << std::endl;
        std::cout << std::endl;
        DebugPrint();
        std::cout << std::endl;

        if (_curTokenMustBeNum) {
            if (IsCharNumeric(ch)) {
                _curTokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else if (ch == '.') {
                // See if a decimal point has already been found
                if (_hasDotBeenFoundOnCurToken) {
                    throw std::runtime_error("A number cannot have two decimal points");
                }
                else {
                    _hasDotBeenFoundOnCurToken = true;
                    _curTokenContents += ch;
                    return AppendingResult::StillWorking;
                }
            }
            else {
                FinishToken();
                std::cout << "CALL `AppendToToken()` INSIDE `AppendToToken()`" << std::endl;
                AppendToToken(ch);
                return AppendingResult::FinishedToken;
            }
        }
        else if (_curTokenMustBeSymbol) {
            if (IsCharAlphabetical(ch)) {
                _curTokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else {
                FinishToken();
                std::cout << "CALL `AppendToToken()` INSIDE `AppendToToken()`" << std::endl;
                AppendToToken(ch);
                return AppendingResult::FinishedToken;
            }
        }
        else if (_hasBackslashBeenFoundOnCurToken) {
            if (IsCharAlphabetical(ch)) {
                _curTokenMustBeSymbol = true;
                _curTokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else if (IsCharNumeric(ch)) {
                throw std::runtime_error("A LaTeX command cannot start with a number");
            }
            else if (IsCharWhitespace(ch)) {
                ClearBuilder();
                return AppendingResult::StillWorking;
            }
            else {
                switch (ch) {
                    case ',':
                    case ':':
                    case ';':
                    case '!':
                        FinishToken();
                        return AppendingResult::StillWorking;
                    case '\\':
                    case '{':
                    case '}':
                        _curTokenMustBeOperator = true;
                        _curTokenContents += ch;

                        FinishToken();
                        return AppendingResult::FinishedToken;
                    default:
                        throw std::runtime_error(std::format("Invalid character ({}) infront of \\", ch));
                }
            }
        }
        else {
            if (IsCharWhitespace(ch) && _curTokenContents.empty()) {
                // Don't append the last space character to the working token
                return AppendingResult::StillWorking;
            }
            else if (ch == '\\') {
                _hasBackslashBeenFoundOnCurToken = true;
                _curTokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else if (ch == '.') {
                _curTokenMustBeNum = true;
                _hasDotBeenFoundOnCurToken = true;
                _curTokenContents += ch;
                return AppendingResult::StillWorking;
            }
            else if (IsCharAlphabetical(ch)) {
                _curTokenMustBeSymbol = true;
                _curTokenContents += ch;
                FinishToken();
                return AppendingResult::FinishedToken;
            }
            else if (IsCharNumeric(ch)) {
                _curTokenMustBeNum = true;
                _curTokenContents += ch;
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
                        _curTokenMustBeOperator = true;
                        _curTokenContents += ch;
                        FinishToken();
                        return AppendingResult::FinishedToken;
                    default:
                        throw std::runtime_error(std::format("Invalid character ({}) in LaTeX", ch));
                }
            }
        }
    }

    std::vector<Token> TokenBuilder::GetTokens() {
        if (!_curTokenContents.empty()) {
            FinishToken();
        }

        return _tokens;
    }

    void TokenBuilder::FinishToken() {
        std::cout << "FINISH TOKEN!" << std::endl;

        Token token(Token::TokenType::Null);

        if (_curTokenMustBeSymbol) {
            token.Type = Token::TokenType::Symbol;
            token.Value = _curTokenContents;
        }
        else if (_curTokenMustBeNum) {
            token.Type = Token::TokenType::Num;
            token.Value = _curTokenContents;
        }
        else if (_curTokenMustBeOperator) {
            if (_curTokenContents == "+") {
                token.Type = Token::TokenType::Plus;
            }
            else if (_curTokenContents == "-") {
                token.Type = Token::TokenType::Minus;
            }
            else if (_curTokenContents == "*") {
                token.Type = Token::TokenType::Star;
            }
            else if (_curTokenContents == "/") {
                token.Type = Token::TokenType::ForwardSlash;
            }
            else if (_curTokenContents == "_") {
                token.Type = Token::TokenType::Underscore;
            }
            else if (_curTokenContents == "^") {
                token.Type = Token::TokenType::UpArrow;
            }
            else if (_curTokenContents == "&") {
                token.Type = Token::TokenType::Ampersand;
            }
            else if (_curTokenContents == "=") {
                token.Type = Token::TokenType::EqualSign;
            }
            else if (_curTokenContents == "\\\\") {
                token.Type = Token::TokenType::LineBreak;
            }
            else if (_curTokenContents == "(") {
                token.Type = Token::TokenType::LeftParenthesis;
            }
            else if (_curTokenContents == ")") {
                token.Type = Token::TokenType::RightParenthesis;
            }
            else if (_curTokenContents == "[") {
                token.Type = Token::TokenType::LeftSquareBracket;
            }
            else if (_curTokenContents == "]") {
                token.Type = Token::TokenType::RightSquareBracket;
            }
            else if (_curTokenContents == "{") {
                token.Type = Token::TokenType::LeftCurlyBracket;
            }
            else if (_curTokenContents == "}") {
                token.Type = Token::TokenType::RightCurlyBracket;
            }
            else if (_curTokenContents == "\\{") {
                token.Type = Token::TokenType::EscapedLeftCurlyBracket;
            }
            else if (_curTokenContents == "\\}") {
                token.Type = Token::TokenType::EscapedRightCurlyBracket;
            }
            else if (_curTokenContents == "<") {
                token.Type = Token::TokenType::LeftAngleBracket;
            }
            else if (_curTokenContents == ">") {
                token.Type = Token::TokenType::RightAngleBracket;
            }
            else {
                throw std::runtime_error("Unknown operator found");
            }
        }
        else {
            throw std::runtime_error("Token not found");
        }

        _tokens.push_back(token);

        ClearBuilder();
    }

    void TokenBuilder::DebugPrint() {
        std::cout << "TOKEN BUILDER:" << std::endl;

        std::cout << "_curTokenContents: " << _curTokenContents << std::endl;
        std::cout << "_curTokenMustBeNum: " << _curTokenMustBeNum << std::endl;
        std::cout << "_curTokenMustBeSymbol: " << _curTokenMustBeSymbol << std::endl;
        std::cout << "_hasDotBeenFoundOnCurToken: " << _hasDotBeenFoundOnCurToken << std::endl;
        std::cout << "_hasBackslashBeenFoundOnCurToken: " << _hasBackslashBeenFoundOnCurToken << std::endl;
        
        std::cout << "_tokens:" << std::endl;
        for (const auto& token : _tokens) {
            std::cout << token.DebugToString() << std::endl;
        }

        if (!_tokens.size()) {
            std::cout << std::endl;
        }
    }

    void TokenBuilder::ClearBuilder() {
        _curTokenContents.clear();
        _curTokenMustBeNum = false;
        _curTokenMustBeSymbol = false;
        _hasDotBeenFoundOnCurToken = false;
        _hasBackslashBeenFoundOnCurToken = false;
    }

    std::vector<Token> TokenizeLatex(std::string_view latex) {
        TokenBuilder builder;

        for (char ch : latex) {
            builder.AppendToToken(ch);
        }

        return builder.GetTokens();
    }
}

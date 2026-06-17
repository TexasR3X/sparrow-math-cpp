#include <format>
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
            case TokenType::ForwardSlash:
                type = "ForwardSlash";
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
        }

        return type + "(" + Value + ")";
    }

    std::vector<Token> TokenizeLatex(std::string_view latex) {
        StrIterator it(latex);
        std::vector<Token> tokens;

        while (it.GetBoolIsNotFinished()) {
            Token token;

            if (it.Peak() == '+') {
                token.Type = Token::TokenType::Plus;
                it.Advance();
            }
            else if (it.Peak() == '-') {
                token.Type = Token::TokenType::Minus;
                it.Advance();
            }
            else if (it.Peak() == '*') {
                token.Type = Token::TokenType::Star;
                it.Advance();
            }
            else if (it.Peak() == '/') {
                token.Type = Token::TokenType::ForwardSlash;
                it.Advance();
            }
            else if (it.Peak() == '_') {
                token.Type = Token::TokenType::Underscore;
                it.Advance();
            }
            else if (it.Peak() == '^') {
                token.Type = Token::TokenType::UpArrow;
                it.Advance();
            }
            else if (it.Peak() == '&') {
                token.Type = Token::TokenType::Ampersand;
                it.Advance();
            }
            else if (it.Peak() == '=') {
                token.Type = Token::TokenType::EqualSign;
                it.Advance();
            }
            else if (it.Peak() == '(') {
                token.Type = Token::TokenType::LeftParenthesis;
                it.Advance();
            }
            else if (it.Peak() == ')') {
                token.Type = Token::TokenType::RightParenthesis;
                it.Advance();
            }
            else if (it.Peak() == '[') {
                token.Type = Token::TokenType::LeftSquareBracket;
                it.Advance();
            }
            else if (it.Peak() == ']') {
                token.Type = Token::TokenType::RightSquareBracket;
                it.Advance();
            }
            else if (it.Peak() == '{') {
                token.Type = Token::TokenType::LeftCurlyBracket;
                it.Advance();
            }
            else if (it.Peak() == '}') {
                token.Type = Token::TokenType::RightCurlyBracket;
                it.Advance();
            }
            else if (it.Peak() == '<') {
                token.Type = Token::TokenType::LeftAngleBracket;
                it.Advance();
            }
            else if (it.Peak() == '>') {
                token.Type = Token::TokenType::RightAngleBracket;
                it.Advance();
            }
            else if (it.Peak() == '\\') {
                it.Advance();

                if (it.GetBoolIsNotFinished()) {
                    if (it.Peak() == '\\') {
                        token.Type = Token::TokenType::LineBreak;
                        it.Advance();
                    }
                    else if (it.Peak() == '{') {
                        token.Type = Token::TokenType::EscapedLeftCurlyBracket;
                        it.Advance();
                    }
                    else if (it.Peak() == '}') {
                        token.Type = Token::TokenType::EscapedRightCurlyBracket;
                        it.Advance();
                    }
                    else if (IsCharWhitespace(it.Peak()) || it.Peak() == ',' || it.Peak() == ':' || it.Peak() == ';' || it.Peak() == '!') {
                        it.Advance();
                        continue;
                    }
                    else if (IsCharAlphabetical(it.Peak())) {
                        token.Type = Token::TokenType::Symbol;

                        while (it.GetBoolIsNotFinished() && IsCharAlphabetical(it.Peak())) {
                            token.Value += it.Advance();
                        }
                    }
                    else {
                        throw std::runtime_error(std::format("Invalid character ({}) after '\\'", it.Peak()));
                    }
                }
                else {
                    throw std::runtime_error("LaTeX cannot end with \\");
                }                
            }
            else if (IsCharWhitespace(it.Peak())) {
                it.Advance();
                continue;
            }
            else if (IsCharNumeric(it.Peak()) || it.Peak() == '.') {
                token.Type = Token::TokenType::Num;
                auto hasDotBeenFound = it.Peak() == '.';

                while (it.GetBoolIsNotFinished() && (IsCharNumeric(it.Peak()) || it.Peak() == '.')) {
                    if (it.Peak() == '.') {
                        if (hasDotBeenFound) {
                            throw std::runtime_error("A number cannot have two decimal points");
                        }
                        else {
                            hasDotBeenFound = true;
                        }
                    }

                    token.Value += it.Advance();
                }
            }
            else if (IsCharAlphabetical(it.Peak())) {
                token.Type = Token::TokenType::Symbol;
                token.Value = it.Advance();
            }

            tokens.push_back(token);
        }

        return tokens;
    }
}

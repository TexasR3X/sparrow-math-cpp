#include <format>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "TokenUtils.hpp"

#define DEBUG_LOG(code) std::cout << #code << ": " << (code) << std::endl

namespace sparrow_math::internal::ParsingUtils {
    std::string Token::ToString() const {
        switch (Type) {
            case TokenType::Number:
                return "Number(" + Value + ")";
            case TokenType::Symbol:
                return "Symbol(" + Value + ")";
            case TokenType::Plus:
                return "Plus";
            case TokenType::Minus:
                return "Minus";
            case TokenType::Star:
                return "Star";
            case TokenType::Underscore:
                return "Underscore";
            case TokenType::UpArrow:
                return "UpArrow";
            case TokenType::LeftParenthesis:
                return "LeftParenthesis";
            case TokenType::RightParenthesis:
                return "RightParenthesis";
            case TokenType::LeftSquareBracket:
                return "LeftSquareBracket";
            case TokenType::RightSquareBracket:
                return "RightSquareBracket";
            case TokenType::LeftCurlyBracket:
                return "LeftCurlyBracket";
            case TokenType::RightCurlyBracket:
                return "RightCurlyBracket";
            default:
                throw std::runtime_error("Unknown `TokenType` option");
        }
    }

    char StringIterator::Peak() const {
        // Get the current character from the string
        return _str.at(_currentIndex);
    }

    char StringIterator::Advance() {
        // Get the current character from the string
        char currentChar = _str.at(_currentIndex);

        // Increment the current index if the iterator isn't finished
        if (!IsFinished()) {
            _currentIndex++;
        }

        // Return the current character
        return currentChar;
    }

    // DEV NOTE: Update `IsFinished()` to only be true after every element has been passed through (even the last one)
    bool StringIterator::IsFinished() const {
        return _currentIndex + 1 == _strLength;
    }

    std::vector<Token> TokenizeLatex(std::string latex) {
        std::vector<Token> tokens;
        StringIterator latexIterator(latex);

        while (!latexIterator.IsFinished()) {
            DEBUG_LOG(latexIterator.IsFinished());
            DEBUG_LOG(latexIterator.Peak());

            char currentChar = latexIterator.Peak();

            if (currentChar == ' ') {
                latexIterator.Advance();
            }
            else if (currentChar == '+') {
                tokens.emplace_back(TokenType::Plus);
                latexIterator.Advance();
            }
            else if (currentChar == '-') {
                tokens.emplace_back(TokenType::Minus);
                latexIterator.Advance();
            }
            else if (currentChar == '*') {
                tokens.emplace_back(TokenType::Star);
                latexIterator.Advance();
            }
            else if (currentChar == '_') {
                tokens.emplace_back(TokenType::Underscore);
                latexIterator.Advance();
            }
            else if (currentChar == '^') {
                tokens.emplace_back(TokenType::UpArrow);
                latexIterator.Advance();
            }
            else if (currentChar == '(') {
                tokens.emplace_back(TokenType::LeftParenthesis);
                latexIterator.Advance();
            }
            else if (currentChar == ')') {
                tokens.emplace_back(TokenType::RightParenthesis);
                latexIterator.Advance();
            }
            else if (currentChar == '[') {
                tokens.emplace_back(TokenType::LeftSquareBracket);
                latexIterator.Advance();
            }
            else if (currentChar == ']') {
                tokens.emplace_back(TokenType::RightSquareBracket);
                latexIterator.Advance();
            }
            else if (currentChar == '{') {
                tokens.emplace_back(TokenType::LeftCurlyBracket);
                latexIterator.Advance();
            }
            else if (currentChar == '}') {
                tokens.emplace_back(TokenType::RightCurlyBracket);
                latexIterator.Advance();
            }
            else if (currentChar == '\\') {
                currentChar = latexIterator.Advance();

                if (latexIterator.IsFinished()) {
                    // DEV NOTE: I need to update this so it throws an error
                }
                else if (currentChar == ' ' || currentChar == ',' || currentChar == ':' || currentChar == ';' || currentChar == '!' || currentChar == '\\') {
                    // If the current character and the one preceeding deal with whitespace, don't tokenize them
                    latexIterator.Advance();
                }
                else if (std::isalpha(currentChar)) {
                    std::string symbolLiteral;

                    while (std::isalpha(latexIterator.Peak())) {
                        symbolLiteral += latexIterator.Advance();

                        if (latexIterator.IsFinished()) {
                            break;
                        }
                    }

                    tokens.emplace_back(TokenType::Symbol, symbolLiteral);
                }
            }
            else if (std::isdigit(currentChar)) {
                std::string numLiteral;
                bool wasDecimalPointFound = false;

                while (!latexIterator.IsFinished()) {
                    currentChar = latexIterator.Peak();

                    if (currentChar == '.' && !wasDecimalPointFound) {
                        numLiteral += latexIterator.Advance();

                        wasDecimalPointFound = true;
                    }
                    else if (std::isdigit(currentChar)) {
                        numLiteral += latexIterator.Advance();
                    }
                    else {
                        break;
                    }
                }

                tokens.emplace_back(TokenType::Number, numLiteral);
            }
            else if (std::isalpha(currentChar)) {
                std::string symbol(1, currentChar);

                tokens.emplace_back(TokenType::Symbol, symbol);
            }
            else {
                throw std::runtime_error(std::format("Cannot tokenize `{}`", currentChar));
            }
        }

        return tokens;
    }
}

#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
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
            case TokenType::DoubleAmpersand:
                type = "DoubleAmpersand";
                break;
            case TokenType::Pipe:
                type = "Pipe";
                break;
            case TokenType::DoublePipe:
                type = "DoublePipe";
                break;
            case TokenType::EqualSign:
                type = "EqualSign";
                break;
            case TokenType::DoubleEqualSign:
                type = "DoubleEqualSign";
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

    Token::TokenType HandleDoubleCharToken(StrIterator& it, char ch, Token::TokenType singleCharType, Token::TokenType doubleCharType) {
        it.Advance();

        if (it.Peak() == ch) {
            it.Advance();

            return doubleCharType;
        }
        else {
            return singleCharType;
        }
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
                token.Type = HandleDoubleCharToken(it, '&', Token::TokenType::Ampersand, Token::TokenType::DoubleAmpersand);
            }
            else if (it.Peak() == '|') {
                token.Type = HandleDoubleCharToken(it, '|', Token::TokenType::Pipe, Token::TokenType::DoublePipe);
            }
            else if (it.Peak() == '=') {
                token.Type = HandleDoubleCharToken(it, '=', Token::TokenType::EqualSign, Token::TokenType::DoubleEqualSign);
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
                        throw std::runtime_error(std::format("Invalid character ({}) after \"\\\"", it.Peak()));
                    }
                }
                else {
                    throw std::runtime_error("LaTeX cannot end with \"\\\"");
                }                
            }
            else if (IsCharWhitespace(it.Peak())) {
                it.Advance();
                continue;
            }
            else if (IsCharNumericOrDot(it.Peak())) {
                token.Type = Token::TokenType::Num;
                auto hasDotBeenFound = it.Peak() == '.';

                while (it.GetBoolIsNotFinished() && IsCharNumericOrDot(it.Peak())) {
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
            else {
                throw std::runtime_error(std::format("Invalid character \"{}\"", it.Peak()));
            }

            tokens.push_back(token);
        }

        return tokens;
    }

    void DebugPrintTokens(std::string_view displayName, const std::vector<Token>& tokens, size_t focusedIndex) {
        std::cout << displayName << ": ";

        if (tokens.empty()) {
            std::cout << "[EMPTY]" << std::endl;
        }
        else {
            for (size_t i = 0; i < tokens.size(); ++i) {
                if (i == focusedIndex) {
                    std::cout << "\033[31m";
                }

                std::cout << tokens.at(i).DebugToString();

                if (i == focusedIndex) {
                    std::cout << "\033[0m";
                }

                if (i == tokens.size() - 1) {
                    std::cout << std::endl;
                }
                else {
                    std::cout << ", ";
                }
            }
        }
    }

    void DelimiterBalanceChecker::ProcessDelimiter(Token::TokenType del) {
        switch (del) {
            case Token::TokenType::LeftParenthesis:
            case Token::TokenType::LeftSquareBracket:
            case Token::TokenType::LeftCurlyBracket:
            case Token::TokenType::EscapedLeftCurlyBracket:
            case Token::TokenType::LeftAngleBracket: {
                _delStack.push(del);
                break;
            }
            case Token::TokenType::RightParenthesis: {
                HandleRightDelimiter(Token::TokenType::LeftParenthesis);
                break;
            }
            case Token::TokenType::RightSquareBracket: {
                HandleRightDelimiter(Token::TokenType::LeftSquareBracket);
                break;
            }
            case Token::TokenType::RightCurlyBracket: {
                HandleRightDelimiter(Token::TokenType::LeftCurlyBracket);
                break;
            }
            case Token::TokenType::EscapedRightCurlyBracket: {
                HandleRightDelimiter(Token::TokenType::EscapedLeftCurlyBracket);
                break;
            }
            case Token::TokenType::RightAngleBracket: {
                HandleRightDelimiter(Token::TokenType::LeftAngleBracket);
                break;
            }
            default: {
                throw std::runtime_error("Code that shouldn't be reached");
            }
        }
    }

    void DelimiterBalanceChecker::EnsureDelimiterStackIsEmpty() const {
        if (!_delStack.empty()) {
            throw std::runtime_error("Delimiters are unbalanced");
        }
    }

    void DelimiterBalanceChecker::HandleRightDelimiter(Token::TokenType leftDel) {
        if (!_delStack.empty() && _delStack.top() == leftDel) {
            _delStack.pop();
        }
        else {
            throw std::runtime_error("Delimiters are unbalanced");
        }
    }

    void CondensePlusAndMinusSigns(std::vector<Token>::iterator& it, std::vector<Token>& tokens, bool removePlusSigns) {
        auto beginningIt = it;
        size_t plusCount = 0;
        size_t minusCount = 0;

        // Count how many pluses and minuses are in the next several tokens
        while (true) {
            if (it == tokens.end()) {
                if (plusCount == 0 && minusCount == 0) {
                    break;
                }
                else {
                    auto plusOrMinus = (it - 1)->Type == Token::TokenType::Plus ? '+' : '-';

                    throw std::runtime_error(std::format("LaTeX cannot end with a \"{}\"", plusOrMinus));
                }
            }
            else if (it->Type == Token::TokenType::Plus) {
                ++plusCount;
            }
            else if (it->Type == Token::TokenType::Minus) {
                ++minusCount;
            }
            else {
                break;
            }

            ++it;
        }

        // If there are no pluses or minuses, move the iterator back one
        if (plusCount == 0 && minusCount == 0) {
            --it;
        }
        else {
            // Remove all the pluses and minuses
            it = tokens.erase(beginningIt, it);

            // If `removePlusSigns` is false, insert a new plus token
            if (!removePlusSigns) {
                it = tokens.insert(it, Token(Token::TokenType::Plus));

                ++it;
            }

            // If the minus count is odd, insert a minus token after the plus
            if (minusCount % 2 == 1) {
                it = tokens.insert(it, Token(Token::TokenType::Minus));

                ++it;
            }
        }
    }

    void CleanTokens(std::vector<Token>& tokens) {
        if (!tokens.empty()) {
            DelimiterBalanceChecker delBalanceChecker;
            auto it = tokens.begin();

            while (it != tokens.end()) {
                DebugPrintTokens("TOKENS BEFORE", tokens, it - tokens.begin());

                if (it->Type == Token::TokenType::Plus || it->Type == Token::TokenType::Minus) {
                    auto removePlusSigns = it == tokens.begin() || (
                        (it - 1)->Type != Token::TokenType::Num
                        && (it - 1)->Type != Token::TokenType::Symbol
                        && !(it - 1)->IsTokenRightDelimiter()
                    );

                    CondensePlusAndMinusSigns(it, tokens, removePlusSigns);
                }
                else if (it->Type == Token::TokenType::Num || it->Type == Token::TokenType::Symbol) {
                    ++it;

                    if (it == tokens.end()) {
                        break;
                    }
                    else if (it->Type == Token::TokenType::Symbol || (it->Type == Token::TokenType::Num && (it - 1)->Type == Token::TokenType::Symbol)) {
                        it = tokens.insert(it, Token(Token::TokenType::Star));

                        ++it;
                    }
                }
                else if (it->IsTokenDelimiter()) {
                    delBalanceChecker.ProcessDelimiter(it->Type);

                    ++it;
                }
                else if (it->IsTokenOperator()) {
                    ++it;

                    if (it == tokens.end()) {
                        throw std::runtime_error("LaTeX cannot end with an operator");
                    }
                    else if (
                        it->Type != Token::TokenType::Num
                        && it->Type != Token::TokenType::Symbol
                        && it->Type != Token::TokenType::Plus
                        && it->Type != Token::TokenType::Minus
                        && !it->IsTokenLeftDelimiter()
                    ) {
                        throw std::runtime_error("Invalid operator after operator");
                    }
                }
                else {
                    throw std::runtime_error("Unexpected token");
                }

                DebugPrintTokens("TOKENS AFTER", tokens, it - tokens.begin());
                std::cout << std::endl;
            }

            delBalanceChecker.EnsureDelimiterStackIsEmpty();
        }
    }
}

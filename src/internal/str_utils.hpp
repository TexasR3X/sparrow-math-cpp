#include <string>

#pragma once
namespace sparrow_math::internal {
    inline constexpr bool IsCharWhitespace(char ch) {
        return ch == ' ' || ch == '\n' ||  ch == '\r' || ch == '\t' || ch == '\f' || ch == '\v';
    }

    inline constexpr bool IsCharNumeric(char ch) {
        return '0' <= ch && ch <= '9';
    }

    inline constexpr bool IsCharAlphabetical(char ch) {
        return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
    }

    std::string TrimSymbolName(const std::string& name);
}

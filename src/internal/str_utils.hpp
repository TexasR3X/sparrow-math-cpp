#include <string>

#pragma once
namespace sparrow_math::internal {
    inline bool IsCharWhitespace(char ch) {
        return ch == ' ' || ch == '\n' ||  ch == '\r' || ch == '\t' || ch == '\f' || ch == '\v';
    }

    inline bool IsCharNumeric(char ch) {
        return '0' <= ch && ch <= '9';
    }

    inline bool IsCharAlphabetical(char ch) {
        return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
    }

    std::string TrimSymbolName(const std::string& name);

    class StrIterator {
    public:
        StrIterator(std::string_view str) : _str(str), _lastIndex(str.length() - 1) {}

        char Peak() const {
            return _str.at(_currentIndex);
        }

        char Advance() {
            auto curChar = Peak();

            ++_currentIndex;

            return curChar;
        }

        bool GetBoolIsNotFinished() const {
            return _currentIndex <= _lastIndex;
        }
    private:
        size_t _currentIndex = 0;
        const size_t _lastIndex;
        const std::string_view _str;
    };
}

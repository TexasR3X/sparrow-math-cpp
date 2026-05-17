#include <format>
#include <stdexcept>
#include <string>
#include "str_utils.hpp"

namespace sparrow_math::internal {
    std::string TrimSymbolName(const std::string& name) {
        #define THROW_INVALID_NAME_ERROR throw std::runtime_error(std::format("Invalid symbol name: \"{}\"", name));

        enum class IterationPhase {
            FoundNothingOrLeadingWhitespace,
            FoundBackslashSignButNoLetters,
            FoundLetters,
            FoundTrailingWhitespace
        };

        std::string trimmedName;
        IterationPhase iterationPhase = IterationPhase::FoundNothingOrLeadingWhitespace;

        for (auto ch : name) {
            switch (iterationPhase) {
                // Found nothing or found leading whitespace
                case IterationPhase::FoundNothingOrLeadingWhitespace:
                    if (IsCharWhitespace(ch)) {
                        // Ignore the whitespace character
                    }
                    else if (ch == '%' || ch == '\\') {
                        iterationPhase = IterationPhase::FoundBackslashSignButNoLetters;
                    }
                    else if (IsCharAlphabetical(ch)) {
                        trimmedName += ch;
                        iterationPhase = IterationPhase::FoundLetters;
                    }
                    else {
                        THROW_INVALID_NAME_ERROR
                    }
                    break;
                // Found percent sign (or backslash) but no letters yet
                case IterationPhase::FoundBackslashSignButNoLetters:
                    if (IsCharAlphabetical(ch)) {
                        trimmedName += ch;
                        iterationPhase = IterationPhase::FoundLetters;
                    }
                    else {
                        THROW_INVALID_NAME_ERROR
                    }
                    break;
                // Found letters
                case IterationPhase::FoundLetters:
                    if (IsCharWhitespace(ch)) {
                        iterationPhase = IterationPhase::FoundTrailingWhitespace;
                    }
                    else if (IsCharAlphabetical(ch)) {
                        trimmedName += ch;
                    }
                    else {
                        THROW_INVALID_NAME_ERROR
                    }
                    break;
                // Found trailing whitespace
                case IterationPhase::FoundTrailingWhitespace:
                    if (!IsCharWhitespace(ch)) {
                        THROW_INVALID_NAME_ERROR
                    }
                    break;
                default:
                    throw std::runtime_error("Line that shouldn't be reached");
            }
        }

        return trimmedName;

        #undef THROW_INVALID_NAME_ERROR
    }
}

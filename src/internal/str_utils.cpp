#include <format>
#include <stdexcept>
#include <string>
#include "str_utils.hpp"

namespace sparrow_math::internal {
    std::string TrimSymbolName(const std::string& name) {
        enum class IterationPhase {
            FoundNothingOrLeadingWhitespace,
            FoundBackslashButNoLetters,
            FoundLetters,
            FoundTrailingWhitespace
        };

        auto ThrowInvalidNameError = [&name] {
            throw std::runtime_error(std::format("Invalid symbol name: \"{}\"", name));
        };

        std::string trimmedName;
        IterationPhase iterationPhase = IterationPhase::FoundNothingOrLeadingWhitespace;

        for (auto ch : name) {
            switch (iterationPhase) {
                case IterationPhase::FoundNothingOrLeadingWhitespace:
                    if (IsCharWhitespace(ch)) {
                        // Ignore the whitespace character
                    }
                    else if (ch == '\\') {
                        iterationPhase = IterationPhase::FoundBackslashButNoLetters;
                    }
                    else if (IsCharAlphabetical(ch)) {
                        trimmedName += ch;
                        iterationPhase = IterationPhase::FoundLetters;
                    }
                    else {
                        ThrowInvalidNameError();
                    }
                    break;
                case IterationPhase::FoundBackslashButNoLetters:
                    if (IsCharAlphabetical(ch)) {
                        trimmedName += ch;
                        iterationPhase = IterationPhase::FoundLetters;
                    }
                    else {
                        ThrowInvalidNameError();
                    }
                    break;
                case IterationPhase::FoundLetters:
                    if (IsCharWhitespace(ch)) {
                        iterationPhase = IterationPhase::FoundTrailingWhitespace;
                    }
                    else if (IsCharAlphabetical(ch)) {
                        trimmedName += ch;
                    }
                    else {
                        ThrowInvalidNameError();
                    }
                    break;
                case IterationPhase::FoundTrailingWhitespace:
                    if (!IsCharWhitespace(ch)) {
                        ThrowInvalidNameError();
                    }
            }
        }

        return trimmedName;
    }
}

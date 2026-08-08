#include "internal/parsing_utils/token_utils.hpp"

namespace sparrow_math::internal::parsing_utils {
    class Operator {
    public:
        enum class OperatorType {
            Sum,
            Product,
            Fraction,
            BooleanAnd,
            BitwiseAnd,
            BooleanOr,
            BitwiseOr,
            BooleanNot,
            BooleanEqual,
            BooleanNotEqual,
            DelimiterGrouping,
            Power,
            Underscore,
            Expr
        };

        Operator(OperatorType type) : _type(type) {}
        Operator(Token::TokenType type) : _type(MapTokenTypeToOperatorType(type)) {}

        bool operator==(const Operator& other) const noexcept {
            return _type == other._type;
        }
        bool operator==(OperatorType otherType) const noexcept {
            return _type == otherType;
        }

        bool operator!=(const Operator& other) const noexcept {
            return _type != other._type;
        }
        bool operator!=(OperatorType otherType) const noexcept {
            return _type != otherType;
        }

        static int GetOrderOfOperationsRank(OperatorType type) noexcept {
            switch (type) {
                case OperatorType::Expr:
                    return 0;

                case OperatorType::DelimiterGrouping:
                    return 1;

                case OperatorType::BooleanAnd:
                case OperatorType::BooleanOr:
                    return 2;

                case OperatorType::BooleanEqual:
                case OperatorType::BooleanNotEqual:
                    return 3;

                case OperatorType::BitwiseAnd:
                case OperatorType::BitwiseOr:
                    return 4;

                case OperatorType::Sum:
                    return 5;

                case OperatorType::Product:
                case OperatorType::Fraction:
                    return 6;

                case OperatorType::Power:
                case OperatorType::Underscore:
                    return 7;

                case OperatorType::BooleanNot:
                    return 8;
            }
        }

        int GetOrderOfOperationsRank() const noexcept {
            return GetOrderOfOperationsRank(_type);
        }

        std::string DebugToString() const noexcept {
            switch (_type) {
                case OperatorType::Sum:
                    return "Sum";

                case OperatorType::Product:
                    return "Product";

                case OperatorType::Fraction:
                    return "Fraction";

                case OperatorType::BooleanAnd:
                    return "BooleanAnd";

                case OperatorType::BitwiseAnd:
                    return "BitwiseAnd";

                case OperatorType::BooleanOr:
                    return "BooleanOr";

                case OperatorType::BitwiseOr:
                    return "BitwiseOr";

                case OperatorType::BooleanNot:
                    return "BooleanNot";

                case OperatorType::BooleanEqual:
                    return "BooleanEqual";

                case OperatorType::BooleanNotEqual:
                    return "BooleanNotEqual";

                case OperatorType::DelimiterGrouping:
                    return "DelimiterGrouping";

                case OperatorType::Power:
                    return "Power";

                case OperatorType::Underscore:
                    return "Underscore";

                case OperatorType::Expr:
                    return "Expr";
            }
        }
    private:
        const OperatorType _type;

        static OperatorType MapTokenTypeToOperatorType(Token::TokenType tokenType) {
            switch (tokenType) {
                case Token::TokenType::DoubleAmpersand:
                    return OperatorType::BooleanAnd;

                case Token::TokenType::DoublePipe:
                    return OperatorType::BooleanOr;

                case Token::TokenType::Ampersand:
                    return OperatorType::BitwiseAnd;

                case Token::TokenType::Pipe:
                    return OperatorType::BitwiseOr;

                case Token::TokenType::Plus:
                    return OperatorType::Sum;

                case Token::TokenType::Star:
                    return OperatorType::Product;

                case Token::TokenType::ForwardSlash:
                    return OperatorType::Fraction;

                case Token::TokenType::UpArrow:
                    return OperatorType::Power;

                default:
                    throw std::runtime_error("Code that shouldn't be reached");
            }
        }
    };
}

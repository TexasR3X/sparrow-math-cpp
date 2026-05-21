#include <format>
#include <memory>
#include <unordered_map>
#include <vector>
#include "token_utils.hpp"

#pragma once
namespace sparrow_math::internal::parsing_utils {
    class Node {
    public:
        enum class NodeType {
            Num,
            Symbol,
            Sum,
            Product,
            Power,
            Expr
        };

        Node* Parent = nullptr;

        virtual ~Node() = default;

        virtual NodeType Type() const = 0;

        virtual Node* GetChildAt(size_t index) const = 0;

        virtual void PushChild(std::unique_ptr<Node> node) = 0;

        virtual std::string DebugToString() const = 0;
    protected:
        static inline std::unordered_map<NodeType, std::string> _nodeTypeMap = {
            { NodeType::Num, "NUM" },
            { NodeType::Symbol, "SYMBOL" },
            { NodeType::Sum, "SUM" },
            { NodeType::Product, "PRODUCT" },
            { NodeType::Power, "POWER" },
            { NodeType::Expr, "EXPR" }
        };
    };

    class NumNode : public Node {
    public:
        long long Num;

        NumNode(long long num) : Node(), Num(num) {}

        NodeType Type() const override {
            return NodeType::Num;
        }

        Node* GetChildAt(size_t index) const override {
            return nullptr;
        }

        void PushChild(std::unique_ptr<Node> node) override {
            throw std::runtime_error("Cannot push child to `NumNode`");
        }

        std::string DebugToString() const override {
            return std::format("{}({})", _nodeTypeMap[NodeType::Num], Num);
        }
    };

    class BranchNode : public Node {
    public:
        BranchNode(NodeType type) : Node(), _type(type) {}

        NodeType Type() const override {
            return _type;
        }

        Node* GetChildAt(size_t index) const override {
            return _children.at(index).get();
        }

        void PushChild(std::unique_ptr<Node> node) override;

        std::string DebugToString() const override {
            std::string name = _nodeTypeMap[Type()];
            std::string childrenStr;

            for (size_t i = 0; i < _children.size(); ++i) {
                if (i) {
                    childrenStr += ", ";
                }

                childrenStr += _children.at(i)->DebugToString();
            }

            return std::format("{}({})", name, childrenStr);
        }
    private:
        NodeType _type;
        std::vector<std::unique_ptr<Node>> _children;
    };

    class SumNode : public BranchNode {
    public:
        SumNode() : BranchNode(NodeType::Sum) {}
    };

    class ProductNode : public BranchNode {
    public:
        ProductNode() : BranchNode(NodeType::Product) {}
    };

    class PowerNode : public BranchNode {
    public:
        PowerNode() : BranchNode(NodeType::Power) {}
    };

    class ExprNode : public BranchNode {
    public:
        ExprNode() : BranchNode(NodeType::Expr) {}
    };

    std::string ParseExpr(std::vector<Token> tokens);
}









// #pragma once
// namespace sparrow_math::internal::parsing_utils {
//     class TokenIterator {
//     public:
//         TokenIterator(std::vector<Token> tokens) : _tokens(tokens) {}

//         Token Peek() {
//             return _tokens.at(_index);
//         }

//         Token MoveForward();

//         Token MoveBackward();
//     private:
//         size_t _index = 0;
//         std::vector<Token> _tokens;
//     };

//     class ExprNode {
//     public:
//         enum class NodeType {
//             Num,
//             Symbol,
//             Sum,
//             Product,
//             Exponent,
//             Fn
//         };
//     };

//     void ParseTokens(std::vector<Token> tokens);
// }










// #pragma once
// namespace sparrow_math::internal::parsing_utils {
//     class ExprNode {
//     public:
//         enum class NodeType {
//             Number,
//             Symbol,
//             Sum,
//             Product,
//             Exponent,
//             SigmaSum,
//             PiProduct
//         };

//         const NodeType Type;
//         // const ExprContext;

//         ExprNode(NodeType type) : Type(type) {}
//     };

//     class NumberNode : public ExprNode {
//     public:
//         double InternalNumber;

//         NumberNode(double number) : ExprNode(NodeType::Number), InternalNumber(number) {}
//     };

//     class SymbolNode : public ExprNode {
//     public:
//         std::string VariableName;

//         SymbolNode(std::string varName) : ExprNode(NodeType::Symbol), VariableName(varName) {}
//     };

//     class OperationNode : public ExprNode {
//     public:
//         const std::vector<ExprNode> const Children;

//         OperationNode(NodeType type) : ExprNode(type) {}
//     };

//     // class DelimiterManager {
//     // public:
//     //     enum class DelimiterType {
//     //         LeftParenthesis,
//     //         RightParenthesis,
//     //         LeftSquareBracket,
//     //         RightSquareBracket,
//     //         LeftCurlyBracket,
//     //         RightCurlyBracket,
//     //         EscapedLeftCurlyBracket,
//     //         EscapedRightCurlyBracket
//     //     };

//     //     DelimiterManager() {}

//     //     void Push(DelimiterType delimiter) {
//     //         _delimiterStack.push(delimiter);
//     //     }
//     // private:
//     //     std::stack<DelimiterType> _delimiterStack;
//     // };

//     void EnsureDelimitersAreBalanced(std::vector<Token>& tokens);

//     std::vector<ExprNode> ParseExpr(std::vector<Token>& tokens);
// }

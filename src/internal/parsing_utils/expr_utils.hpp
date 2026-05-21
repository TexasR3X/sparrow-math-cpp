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

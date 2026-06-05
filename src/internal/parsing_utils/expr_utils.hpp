#include <format>
#include <memory>
#include <unordered_map>
#include <vector>
#include "token_utils.hpp"

#pragma once
namespace sparrow_math::internal::parsing_utils {
    class Node;
    class BranchNode;

    class Node {
    public:
        enum class NodeType {
            Num,
            Symbol,
            Sum,
            Product,
            Power,
            Delimiters,
            Expr
        };

        static int GetNodeTypeRank(NodeType type) {
            return (int)type;
        }

        BranchNode* Parent = nullptr;

        virtual ~Node() = default;

        virtual NodeType Type() const = 0;

        virtual Node* GetChildAt(size_t index) const = 0;

        virtual Node* LastChild() const = 0;

        virtual void AppendChild(std::unique_ptr<Node> node) = 0;

        virtual std::unique_ptr<Node> RemoveLastChild() = 0;

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
        double Num;

        NumNode(double num) : Node(), Num(num) {}

        NodeType Type() const override {
            return NodeType::Num;
        }

        Node* GetChildAt(size_t index) const override {
            return nullptr;
        }

        Node* LastChild() const override {
            return nullptr;
        }

        void AppendChild(std::unique_ptr<Node> node) override {
            throw std::runtime_error("Cannot append a child node to `NumNode`");
        }

        std::unique_ptr<Node> RemoveLastChild() override {
            throw std::runtime_error("Cannot remove a child node to `NumNode`");
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

        Node* LastChild() const override {
            return GetChildAt(_children.size() - 1);
        }

        void AppendChild(std::unique_ptr<Node> node) override;

        std::unique_ptr<Node> RemoveLastChild() override;

        // BranchNode* FindGroupingAncestor() const;

        BranchNode* FindAncestorOfType(const NodeType& type, const bool& stayWithinDels) const;

        BranchNode* FindAncestorOrSelfOfType(const NodeType& type, const bool& stayWithinDels);

        void InsertParentBetweenSelfAndLastChild(std::unique_ptr<BranchNode> parent);

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

    std::string ParseExpr(const std::vector<Token>& tokens);
}

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
            Sum,
            Product,
            Power,
            DelimiterGrouping,
            Expr,
            Symbol,
            Num
        };

        const NodeType Type;

        BranchNode* Parent = nullptr;

        Node(const NodeType& type) : Type(type) {}

        virtual ~Node() = default;

        virtual std::string DebugToString() const = 0;
    protected:
        static inline std::unordered_map<NodeType, std::string> _nodeTypeMap = {
            { NodeType::Num, "Num" },
            { NodeType::Symbol, "Symbol" },
            { NodeType::Sum, "Sum" },
            { NodeType::Product, "Product" },
            { NodeType::Power, "Power" },
            { NodeType::Expr, "Expr" }
        };
    };

    class NumNode : public Node {
    public:
        double Num;

        NumNode(double num) : Node(NodeType::Num), Num(num) {}

        std::string DebugToString() const override;
    };

    class BranchNode : public Node {
    public:
        BranchNode(NodeType type) : Node(type) {}

        virtual ~BranchNode() = default;

        Node* GetChildAt(size_t index) const;

        Node* GetLastChild() const;

        void AppendChild(std::unique_ptr<Node> node);

        std::unique_ptr<Node> RemoveLastChild();

        BranchNode* GetNearestAncestorOfType(NodeType type, bool stayWithinDels) const;

        BranchNode* GetNearestAncestorOrSelfOfType(NodeType type, bool stayWithinDels);

        BranchNode* GetNearestGroupingAncestorOrSelf();

        std::string DebugToString() const override;
    private:
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

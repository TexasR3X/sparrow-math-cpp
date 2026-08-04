#include <memory>
#include <vector>
#include "token_utils.hpp"
#include "operator_utils.hpp"

#pragma once
namespace sparrow_math::internal::parsing_utils {
    class Node;
    class BranchNode;

    class Node {
    public:
        enum class NodeType {
            Num,
            Symbol,
            OperatorNode,
            Expr
        };

        // enum class NodeType {
        //     Num,
        //     Symbol,
        //     Sum,
        //     Product,
        //     Fraction,
        //     Power,
        //     BooleanAnd,
        //     BitwiseAnd,
        //     BooleanOr,
        //     BitwiseOr,
        //     BooleanNot,
        //     BooleanEqual,
        //     BooleanNotEqual,
        //     DelimiterGrouping,
        //     Expr
        // };

        const NodeType Type;

        BranchNode* Parent = nullptr;

        Node(NodeType type) : Type(type) {}

        virtual ~Node() = default;

        template<typename T>
        // requires std::derived_from<T, Node>
        T* CastAsType() {
            return dynamic_cast<T*>(this);
        }

        virtual std::string DebugToString() = 0;
    // protected:
    //     static inline std::unordered_map<NodeType, std::string> _nodeTypeMap = {
    //         { NodeType::Num, "Num" },
    //         { NodeType::Symbol, "Symbol" },
    //         // { NodeType::Sum, "Sum" },
    //         // { NodeType::Product, "Product" },
    //         // { NodeType::Power, "Power" },
    //         // { NodeType::Expr, "Expr" }
    //     };
    };

    class NumNode : public Node {
    public:
        double Num;

        NumNode(double num) : Node(NodeType::Num), Num(num) {}

        std::string DebugToString() override;
    };

    class SymbolNode : public Node {
    public:
        std::string Name;

        SymbolNode(std::string_view name) : Node(NodeType::Symbol), Name(name) {}

        std::string DebugToString() override;
    };

    class BranchNode : public Node {
    public:
        BranchNode(NodeType type) : Node(type) {}

        virtual ~BranchNode() = default;

        Node* GetChildAt(size_t index) const;

        Node* GetLastChild() const;

        void AppendChild(std::unique_ptr<Node> node);

        std::unique_ptr<Node> RemoveLastChild();

        BranchNode* GetNearestAncestorOfType(const Operator& type, bool stayWithinDels) const;

        BranchNode* GetNearestAncestorOrSelfOfType(const Operator& type, bool stayWithinDels);

        BranchNode* GetNearestGroupingAncestorOrSelf();

        std::string DebugToString() override;
    protected:
        std::vector<std::unique_ptr<Node>> _children;
    };

    class OperatorNode : public BranchNode {
    public:
        Operator Op;

        OperatorNode(Operator op) : BranchNode(NodeType::OperatorNode), Op(op) {}
    };

    class ExprNode : public BranchNode {
    public:
        ExprNode() : BranchNode(NodeType::Expr) {}
    };

    std::string ParseExpr(const std::vector<Token>& tokens);
}

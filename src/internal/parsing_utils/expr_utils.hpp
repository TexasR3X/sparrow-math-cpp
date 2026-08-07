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
        BranchNode* Parent = nullptr;

        virtual ~Node() = default;

        template<typename T>
        requires std::derived_from<T, Node>
        T* CastAsType() {
            return dynamic_cast<T*>(this);
        }

        virtual std::string DebugToString() = 0;
    };

    class NumNode : public Node {
    public:
        double Num;

        NumNode(double num) : Num(num) {}

        std::string DebugToString() override;
    };

    class SymbolNode : public Node {
    public:
        std::string Name;

        SymbolNode(std::string_view name) : Name(name) {}

        std::string DebugToString() override;
    };

    class BranchNode : public Node {
    public:
        Operator Op;

        BranchNode(Operator op) : Op(op) {}

        virtual ~BranchNode() = default;

        Node* GetChildAt(size_t index) const;

        Node* GetLastChild() const;

        void AppendChild(std::unique_ptr<Node> node);

        std::unique_ptr<Node> RemoveLastChild();

        BranchNode* GetNearestAncestorWithOp(const Operator& type, bool stayWithinDels) const;

        BranchNode* GetNearestAncestorOrSelfWithOp(const Operator& type, bool stayWithinDels);

        BranchNode* GetNearestGroupingAncestorOrSelf();

        std::string DebugToString() override;
    protected:
        std::vector<std::unique_ptr<Node>> _children;
    };

    std::string ParseExpr(const std::vector<Token>& tokens);
}

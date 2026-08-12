#include <memory>
#include <tuple>
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

        virtual bool operator==(const Node& other) const;
        bool operator!=(const Node& other) const;

        std::unique_ptr<Node> operator+(const Node& other) const;
        std::unique_ptr<Node> operator+(double num) const;

        std::unique_ptr<Node> operator*(const Node& other) const;
        std::unique_ptr<Node> operator*(double num) const;

        std::unique_ptr<Node> operator^(const Node& other) const;
        std::unique_ptr<Node> operator^(double num) const;

        virtual std::unique_ptr<Node> Clone() const = 0;

        template<typename T>
        requires std::derived_from<T, Node>
        T* CastAsType() const noexcept {
            return dynamic_cast<T*>(this);
        }

        virtual std::unique_ptr<Node> SimplifyNode() const = 0;

        virtual std::string DebugToString() = 0;
    };

    class NumNode : public Node {
    public:
        double Num;

        NumNode(double num) : Num(num) {}

        bool operator==(const Node& other) const override;

        std::unique_ptr<Node> Clone() const override;

        std::unique_ptr<Node> SimplifyNode() const override;

        std::string DebugToString() override;
    };

    class SymbolNode : public Node {
    public:
        std::string Name;

        SymbolNode(std::string_view name) : Name(name) {}

        bool operator==(const Node& other) const override;

        std::unique_ptr<Node> Clone() const override;

        std::unique_ptr<Node> SimplifyNode() const override;

        std::string DebugToString() override;
    };

    class BranchNode : public Node {
    public:
        Operator Op;

        BranchNode(Operator op) : Op(op) {}

        bool operator==(const Node& other) const override;

        std::unique_ptr<Node> Clone() const override;

        Node* GetChildAt(size_t index) const;

        std::unique_ptr<Node> CloneChildAt(size_t index) const;

        Node* GetLastChild() const;

        std::unique_ptr<Node> CloneLastChild() const;

        void AppendChild(std::unique_ptr<Node> node);

        std::unique_ptr<Node> RemoveLastChild();

        BranchNode* GetNearestAncestorWithOp(const Operator& op, bool stayWithinDels) const;

        BranchNode* GetNearestAncestorOrSelfWithOp(const Operator& op, bool stayWithinDels);

        BranchNode* GetNearestGroupingAncestorOrSelf();

        BranchNode* GetAncestorWhereToInsertOperator(const Operator& opToInsert, bool useLeftToRightPriority);

        std::tuple<Node*, Node*> TryToDisassemblePower() const;

        std::unique_ptr<Node> SimplifyNode() const override;

        std::string DebugToString() override;
    protected:
        std::vector<std::unique_ptr<Node>> _children;
    };

    std::unique_ptr<BranchNode> ParseExpr(const std::vector<Token>& tokens);
}

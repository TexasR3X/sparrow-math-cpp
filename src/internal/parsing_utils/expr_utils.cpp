#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include "token_utils.hpp"
#include "expr_utils.hpp"

namespace sparrow_math::internal::parsing_utils {
    // // ========== Local Functions ========== //

    // Create a new node inbetween the focus and its last child. Then move the focus to
    // the new node (reassign the variable `focus` to store the address of the new node).
    void InsertNodeBetweenFocusAndLastChildThenMoveFocus(BranchNode*& focus, const Operator& nodeOp) {
        // Create the new node
        auto newNode = std::make_unique<BranchNode>(nodeOp);

        // Save the address of the new node as a raw pointer
        auto newNodeRawPtr = newNode.get();

        // Remove the last child from the focus
        auto lastChild = focus->RemoveLastChild();

        // Append that child to its new node
        newNode->AppendChild(std::move(lastChild));

        // Append the new node to the focus
        focus->AppendChild(std::move(newNode));

        // Move the focus to the new node
        focus = newNodeRawPtr;
    }

    std::unique_ptr<Node> TryToMergeNodesAsProduct(std::unique_ptr<Node> node1, std::unique_ptr<Node> node2) {
        // Attempt to cast `node1` and `node2` as one of the following types
        auto node1AsNum = node1->CastAsType<NumNode>();
        auto node2AsNum = node2->CastAsType<NumNode>();

        auto node1AsSymbol = node1->CastAsType<SymbolNode>();
        auto node2AsSymbol = node2->CastAsType<SymbolNode>();

        auto node1AsBranch = node1->CastAsType<BranchNode>();
        auto node2AsBranch = node2->CastAsType<BranchNode>();

        if (node1AsNum && node2AsNum) {
            // Get the product of the two numbers
            auto num = node1AsNum->Num * node2AsNum->Num;

            // Return the merged node
            return std::make_unique<NumNode>(num);
        }
        else if (node1AsSymbol && node2AsSymbol && node1AsSymbol->Name == node2AsSymbol->Name) {
            return *node1AsSymbol ^ 2.0;
        }
        else if (node1AsBranch && node1AsBranch->Op == Operator::OperatorType::Power) {
            auto [node1Base, node1Exp] = node1AsBranch->TryToDisassemblePower();

            if (node1Base && node1Exp) {
                if (node2AsBranch && node2AsBranch->Op == Operator::OperatorType::Power) {
                    auto [node2Base, node2Exp] = node2AsBranch->TryToDisassemblePower();

                    if (node2Base && node2Exp && *node1Base == *node2Base) {
                        return *node1Base ^ *(*node1Exp + *node2Exp);
                    }
                }
                else if (*node1Base == *node2) {
                    return *node1Base ^ *(*node1Exp + 1.0);
                }
            }
        }
        else if (node2AsBranch && node2AsBranch->Op == Operator::OperatorType::Power) {
            auto [node2Base, node2Exp] = node2AsBranch->TryToDisassemblePower();

            if (node2Base && node2Exp && *node1 == *node2Base) {
                return *node2Base ^ *(*node2Exp + 1.0);
            }
        }

        return nullptr;
    }



    // ========== Node ========== //

    bool Node::operator!=(const Node& other) const {
        return !(*this == other);
    }

    std::unique_ptr<Node> Node::operator+(const Node& other) const {
        auto sumNode = std::make_unique<BranchNode>(Operator::OperatorType::Sum);

        sumNode->AppendChild(Clone());
        sumNode->AppendChild(other.Clone());

        return sumNode->SimplifyNode();
    }
    std::unique_ptr<Node> Node::operator+(double num) const {
        auto numNode = std::make_unique<NumNode>(num);

        return *this + *numNode;
    }

    std::unique_ptr<Node> Node::operator*(const Node& other) const {
        auto productNode = std::make_unique<BranchNode>(Operator::OperatorType::Product);

        productNode->AppendChild(Clone());
        productNode->AppendChild(other.Clone());

        return productNode->SimplifyNode();
    }
    std::unique_ptr<Node> Node::operator*(double num) const {
        auto numNode = std::make_unique<NumNode>(num);

        return *this * *numNode;
    }

    std::unique_ptr<Node> Node::operator^(const Node& other) const {
        auto powerNode = std::make_unique<BranchNode>(Operator::OperatorType::Power);

        powerNode->AppendChild(Clone());
        powerNode->AppendChild(other.Clone());

        return powerNode->SimplifyNode();
    }
    std::unique_ptr<Node> Node::operator^(double num) const {
        auto numNode = std::make_unique<NumNode>(num);

        return *this ^ *numNode;
    }



    // ========== NumNode ========== //

    bool NumNode::operator==(const Node& other) const {
        auto otherNumNode = CastAsType<NumNode>();

        return otherNumNode && Num == otherNumNode->Num;
    }

    std::unique_ptr<Node> NumNode::Clone() const {
        return std::make_unique<NumNode>(*this);
    }

    std::unique_ptr<Node> NumNode::SimplifyNode() const {
        return Clone();
    }

    std::string NumNode::DebugToString() {
        return std::format("Num({})", Num);
    }



    // ========== SymbolNode ========== //

    bool SymbolNode::operator==(const Node& other) const {
        auto otherSymbolNode = CastAsType<SymbolNode>();

        return otherSymbolNode && Name == otherSymbolNode->Name;
    }

    std::unique_ptr<Node> SymbolNode::Clone() const {
        return std::make_unique<SymbolNode>(*this);
    }

    std::unique_ptr<Node> SymbolNode::SimplifyNode() const {
        return Clone();
    }

    std::string SymbolNode::DebugToString() {
        return std::format("Symbol({})", Name);
    }



    // ========== BranchNode ========== //

    bool BranchNode::operator==(const Node& other) const {
        auto otherBranchNode = CastAsType<BranchNode>();

        if (otherBranchNode && _children.size() == otherBranchNode->_children.size()) {
            for (size_t i = 0; i < _children.size(); ++i) {
                auto childOfThis = _children.at(i).get();
                auto childOfOther = otherBranchNode->_children.at(i).get();

                if (childOfThis != childOfOther) {
                    return false;
                }
            }

            return true;
        }
        else {
            return false;
        }
    }

    std::unique_ptr<Node> BranchNode::Clone() const {
        auto clonedBranchNode = std::make_unique<BranchNode>(Op);

        for (const auto& child : _children) {
            auto clonedChild = child->Clone();

            clonedBranchNode->AppendChild(std::move(clonedChild));
        }

        return std::move(clonedBranchNode);
    }

    Node* BranchNode::GetChildAt(size_t index) const {
        return _children.at(index).get();
    }

    std::unique_ptr<Node> BranchNode::CloneChildAt(size_t index) const {
        return _children.at(index)->Clone();
    }

    Node* BranchNode::GetLastChild() const {
        return _children.back().get();
    }

    std::unique_ptr<Node> BranchNode::CloneLastChild() const {
        return _children.back()->Clone();
    }

    void BranchNode::AppendChild(std::unique_ptr<Node> node) {
        if (this == node.get()) {
            throw std::runtime_error("Internal Error: A parent node cannot contain a child node equal to itself");
        }

        node->Parent = this;

        _children.push_back(std::move(node));
    }

    std::unique_ptr<Node> BranchNode::RemoveLastChild() {
        auto lastChild = std::move(_children.back());

        _children.pop_back();

        return std::move(lastChild);
    }

    BranchNode* BranchNode::GetNearestAncestorWithOp(const Operator& op, bool stayWithinDels) const {
        if (!Parent || (stayWithinDels && (Op == Operator::OperatorType::DelimiterGrouping || Parent->Op == Operator::OperatorType::DelimiterGrouping))) {
            return nullptr;
        }
        else if (Parent->Op == op) {
            return Parent;
        }
        else {
            return Parent->GetNearestAncestorWithOp(op, stayWithinDels);
        }
    }

    BranchNode* BranchNode::GetNearestAncestorOrSelfWithOp(const Operator& op, bool stayWithinDels) {
        return Op == op ? this : (
            stayWithinDels && Op == Operator::OperatorType::DelimiterGrouping ? nullptr : GetNearestAncestorWithOp(op, stayWithinDels)
        );
    }

    BranchNode* BranchNode::GetNearestGroupingAncestorOrSelf() {
        if (Op == Operator::OperatorType::DelimiterGrouping || Op == Operator::OperatorType::Expr) {
            return this;
        }
        else if (Parent) {
            return Parent->GetNearestGroupingAncestorOrSelf();
        }
        else {
            throw std::runtime_error("Code that shouldn't be reached; Node has no grouping ancestor");
        }
    }

    BranchNode* BranchNode::GetAncestorWhereToInsertOperator(const Operator& opToInsert, bool useLeftToRightPriority) {
        if (Op.GetOrderOfOperationsRank() > opToInsert.GetOrderOfOperationsRank() || (
            useLeftToRightPriority && Op.GetOrderOfOperationsRank() == opToInsert.GetOrderOfOperationsRank()
        )) {
            if (Parent) {
                return Parent->GetAncestorWhereToInsertOperator(opToInsert, useLeftToRightPriority);
            }
            else {
                throw std::runtime_error("Code that shouldn't be reached; `Parent` is `nullptr`");
            }
        }
        else {
            return this;
        }
    }

    std::tuple<Node*, Node*> BranchNode::TryToDisassemblePower() const {
        if (Op == Operator::OperatorType::Power && _children.size() == 2) {
            auto baseNode = GetChildAt(0);
            auto expNode = GetLastChild();

            return { baseNode, expNode };
        }
        else {
            return { nullptr, nullptr };
        }
    }

    std::unique_ptr<Node> BranchNode::SimplifyNode() const {
        std::unique_ptr<BranchNode> simplifiedNode;

        if (_children.empty()) {
            throw std::runtime_error("Unfinished code; Empty `BranchNode`");
        }
        else if (Op == Operator::OperatorType::Sum) {

        }
        else if (Op == Operator::OperatorType::Product) {
            // Spread out the product children in this product, while simplifying each child
            std::vector<std::unique_ptr<Node>> spreadOutChildren;

            for (size_t i = 0; i < _children.size(); ++i) {
                auto node = CloneChildAt(i);

                node = node->SimplifyNode();

                if (auto nodeAsProduct = node->CastAsType<BranchNode>(); nodeAsProduct && nodeAsProduct->Op == Operator::OperatorType::Product) {
                    while (!nodeAsProduct->_children.empty()) {
                        spreadOutChildren.push_back(nodeAsProduct->RemoveLastChild());
                    }
                }
                else {
                    spreadOutChildren.push_back(std::move(node));
                }
            }

            // Merge all the children that can be merged
            auto firstChild = spreadOutChildren.at(0)->Clone();
            simplifiedNode->AppendChild(std::move(firstChild));

            for (size_t i = 1; i < _children.size(); ++i) {
                auto node1 = spreadOutChildren.at(i - 1)->Clone();
                auto node2 = spreadOutChildren.at(i)->Clone();

                node1 = node1->SimplifyNode();
                node2 = node2->SimplifyNode();

                if (auto mergedNode = TryToMergeNodesAsProduct(std::move(node1), std::move(node2))) {
                    simplifiedNode->AppendChild(std::move(mergedNode));
                }
            }
        }
        else if (Op == Operator::OperatorType::Power) {
            auto [baseNode, expNode] = TryToDisassemblePower();

            if (baseNode && expNode) {
                
            }
            else {
                throw std::runtime_error("Power can only have two operands");
            }
        }

        return nullptr;
    }

    std::string BranchNode::DebugToString() {
        std::string childrenStr;

        for (size_t i = 0; i < _children.size(); ++i) {
            if (i) {
                childrenStr += ", ";
            }

            childrenStr += _children.at(i)->DebugToString();
        }

        return std::format("{}({})", Op.DebugToString(), childrenStr);
    }



    // ========== Public Non-method Functions ========== //

    std::unique_ptr<BranchNode> ParseExpr(const std::vector<Token>& tokens) {
        auto exprNode = std::make_unique<BranchNode>(Operator::OperatorType::Expr);
        auto focus = exprNode.get();

        for (const auto& currentToken : tokens) {
            if (currentToken.Type == Token::TokenType::Num) {
                // Turn the token's string value into a number
                auto num = std::stod(currentToken.Value);

                // Create a `NumNode` from the number
                auto numNode = std::make_unique<NumNode>(num);

                // Append `numNode` to the focus
                focus->AppendChild(std::move(numNode));
            }
            else if (currentToken.Type == Token::TokenType::Symbol) {
                // Create a `SymbolNode` from the symbol token
                auto symbolNode = std::make_unique<SymbolNode>(currentToken.Value);

                // Append `symbolNode` to the focus
                focus->AppendChild(std::move(symbolNode));
            }
            else if (currentToken.Type == Token::TokenType::Underscore) {
                throw std::runtime_error("Unfinished code");
            }
            else if (currentToken.Type == Token::TokenType::ExclamationMark) {
                throw std::runtime_error("Unfinished code");
            }
            else if (currentToken.IsTokenLeftDelimiter()) {
                // Create a delimiter grouping node
                auto del = std::make_unique<BranchNode>(Operator::OperatorType::DelimiterGrouping);

                // Store the delimiter as a raw pointer
                auto delRawPtr = del.get();

                // Append the delimiter to the current focus
                focus->AppendChild(std::move(del));

                // Move the focus to the delimiter
                focus = delRawPtr;
            }
            else if (currentToken.IsTokenRightDelimiter()) {
                // Get the parent of the nearest delimiter ancestor
                auto delParent = focus->GetNearestGroupingAncestorOrSelf()->Parent;

                // Get the nearest delimiter and remove it from its parent
                auto del = delParent->RemoveLastChild();

                if (auto delAsBranchNode = del->CastAsType<BranchNode>()) {
                    // Remove the child from the delimiter
                    auto delChild = delAsBranchNode->RemoveLastChild();

                    // Append the child to the delimiter's former parent
                    delParent->AppendChild(std::move(delChild));

                    // Move the focus to the delimiter's former parent
                    focus = delParent;
                }
                else {
                    throw std::runtime_error("Code that shouldn't be reached");
                }
            }
            else {
                const Operator currentTokenOp(currentToken.Type);

                // Move the focus to the ancestor where the new operator should be inserted
                focus = focus->GetAncestorWhereToInsertOperator(currentTokenOp, currentTokenOp != Operator::OperatorType::Power);

                InsertNodeBetweenFocusAndLastChildThenMoveFocus(focus, currentTokenOp);
            }

            std::cout << "currentToken: " << currentToken.DebugToString() << std::endl;
            std::cout << "exprNode: " << exprNode->DebugToString() << std::endl;
            std::cout << "focus: " << focus->DebugToString() << std::endl;
            std::cout << std::endl;
        }

        return std::move(exprNode);
    }
}

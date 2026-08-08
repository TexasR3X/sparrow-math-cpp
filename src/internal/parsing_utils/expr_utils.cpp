#include <cstddef>
#include <format>
#include <iostream>
#include <stdexcept>
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



    // ========== NumNode ========== //

    std::string NumNode::DebugToString() {
        return std::format("Num({})", Num);
    }



    // ========== SymbolNode ========== //

    std::string SymbolNode::DebugToString() {
        return std::format("Symbol({})", Name);
    }



    // ========== BranchNode ========== //

    Node* BranchNode::GetChildAt(size_t index) const {
        return _children.at(index).get();
    }

    Node* BranchNode::GetLastChild() const {
        return _children.back().get();
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
                const auto& focusOp = focus->Op;

                if (currentTokenOp > focusOp) {
                    InsertNodeBetweenFocusAndLastChildThenMoveFocus(focus, currentTokenOp);
                }
                else if (currentTokenOp < focusOp) {
                    if (auto ancestor = focus->GetNearestAncestorOrSelfWithOp(currentTokenOp, true)) {
                        // Move the focus to the ancestor with the operator of `currentTokenOp`
                        focus = ancestor;
                    }
                    else {
                        // Move the focus to the ancestor
                        focus = focus->GetNearestGroupingAncestorOrSelf();

                        InsertNodeBetweenFocusAndLastChildThenMoveFocus(focus, currentTokenOp);
                    }
                }
            }

            std::cout << "currentToken: " << currentToken.DebugToString() << std::endl;
            std::cout << "exprNode: " << exprNode->DebugToString() << std::endl;
            std::cout << "focus: " << focus->DebugToString() << std::endl;
            std::cout << std::endl;
        }

        return std::move(exprNode);
    }
}

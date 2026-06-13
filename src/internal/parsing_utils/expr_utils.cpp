#include <cstddef>
#include <format>
#include <iostream>
#include <stdexcept>
#include "token_utils.hpp"
#include "expr_utils.hpp"

namespace sparrow_math::internal::parsing_utils {
    // ========== Local Functions ========== //

    Node::NodeType MapTokenTypeToNodeType(const Token::TokenType& tokenType) {
        switch (tokenType) {
            case Token::TokenType::Num:
                return Node::NodeType::Num;
            case Token::TokenType::Plus:
                return Node::NodeType::Sum;
            case Token::TokenType::Star:
                return Node::NodeType::Product;
            case Token::TokenType::UpArrow:
                return Node::NodeType::Power;
            default:
                throw std::runtime_error("Unfinished code in `MapTokenTypeToNodeType()`");
        }
    }

    // Create a new node inbetween the focus and its last child. Then move the focus to
    // the new node (reassign the variable `focus` to store the address of the new node).
    void InsertNodeBetweenFocusAndLastChildThenMoveFocus(BranchNode*& focus, Node::NodeType nodeType) {
        // Create the new node
        auto newNode = std::make_unique<BranchNode>(nodeType);

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

    std::string NumNode::DebugToString() const {
        return std::format("{}({})", _nodeTypeMap[NodeType::Num], Num);
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

    BranchNode* BranchNode::GetNearestAncestorOfType(const NodeType& type, const bool& stayWithinDels) const {
        if (!Parent || (stayWithinDels && Parent->Type == NodeType::DelimiterGrouping && type != NodeType::DelimiterGrouping)) {
            return nullptr;
        }
        else if (Parent->Type == type) {
            return Parent;
        }
        else {
            return Parent->GetNearestAncestorOfType(type, stayWithinDels);
        }
    }

    BranchNode* BranchNode::GetAncestorOrSelfOfType(const NodeType& type, const bool& stayWithinDels) {
        return Type == type ? this : GetNearestAncestorOfType(type, stayWithinDels);
    }

    BranchNode* BranchNode::GetNearestGroupingAncestorOrSelf() {
        if (Type == NodeType::Expr || Type == NodeType::DelimiterGrouping) {
            return this;
        }
        else if (!Parent) {
            throw std::runtime_error("Code that shouldn't be reached; Node has no grouping ancestor");
        }
        else {
            return Parent->GetNearestGroupingAncestorOrSelf();
        }
    }

    std::string BranchNode::DebugToString() const {
        std::string name = _nodeTypeMap[Type];
        std::string childrenStr;

        for (size_t i = 0; i < _children.size(); ++i) {
            if (i) {
                childrenStr += ", ";
            }

            childrenStr += _children.at(i)->DebugToString();
        }

        return std::format("{}({})", name, childrenStr);
    }



    // ========== Public Non-method Functions ========== //

    std::string ParseExpr(const std::vector<Token>& tokens) {
        auto exprNode = std::make_unique<ExprNode>();
        BranchNode* focus = exprNode.get();

        for (const auto& currentToken : tokens) {
            auto currentNodeType = MapTokenTypeToNodeType(currentToken.Type);

            if (currentNodeType == Node::NodeType::Num) {
                // Turn the token's string value into a number
                auto num = std::stod(currentToken.Value);

                // Create a `NumNode` from the number
                auto numNode = std::make_unique<NumNode>(num);

                // Append `numNode` to the focus
                focus->AppendChild(std::move(numNode));
            }
            else if ((int)currentNodeType > (int)focus->Type) {
                InsertNodeBetweenFocusAndLastChildThenMoveFocus(focus, currentNodeType);
            }
            else if ((int)currentNodeType < (int)focus->Type) {
                if (auto ancestor = focus->GetNearestAncestorOfType(currentNodeType, true)) {
                    // Move the focus to the ancestor of type `currentNodeType`
                    focus = ancestor;
                }
                else {
                    // Move the focus to the ancestor
                    focus = focus->GetNearestGroupingAncestorOrSelf();

                    InsertNodeBetweenFocusAndLastChildThenMoveFocus(focus, currentNodeType);
                }
            }

            std::cout << "currentToken: " << currentToken.DebugToString() << std::endl;
            std::cout << "exprNode: " << exprNode->DebugToString() << std::endl;
            std::cout << "focus: " << focus->DebugToString() << std::endl;
            std::cout << std::endl;
        }

        return exprNode->DebugToString();
    }
}

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

    void InsertNodeBetweenFocusAndLastChildThenMoveFocus(BranchNode*& focus, std::unique_ptr<BranchNode> newNode) {
        // Save the address of the new node as a raw pointer
        auto newNodeRawPtr = newNode.get();

        // Remove the last child from the focus
        auto lastChild = focus->RemoveLastChild();

        // Append that child to its new node
        newNode->AppendChild(std::move(lastChild));

        // Append the new node to the focus
        focus->AppendChild(std::move(newNode));

        // Move focus on the new node
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

    Node* BranchNode::LastChild() const {
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

    BranchNode* BranchNode::FindAncestorOfType(const NodeType& type, const bool& stayWithinDels) const {
        if (!Parent || (stayWithinDels && Parent->Type == NodeType::DelimiterGrouping && type != NodeType::DelimiterGrouping)) {
            return nullptr;
        }
        else if (Parent->Type == type) {
            return Parent;
        }
        else {
            return Parent->FindAncestorOfType(type, stayWithinDels);
        }
    }

    BranchNode* BranchNode::FindAncestorOrSelfOfType(const NodeType& type, const bool& stayWithinDels) {
        return Type == type ? this : FindAncestorOfType(type, stayWithinDels);
    }

    BranchNode* BranchNode::FindGroupingAncestor() const {
        if (!Parent) {
            throw std::runtime_error("Code that shouldn't be reached; Node has no grouping ancestor");
        }
        else if (Parent->Type == NodeType::Expr || Parent->Type == NodeType::DelimiterGrouping) {
            return Parent;
        }
        else {
            return Parent->FindGroupingAncestor();
        }
    }

    BranchNode* BranchNode::FindGroupingAncestorOrSelf() {
        if (Type == NodeType::Expr || Type == NodeType::DelimiterGrouping) {
            return this;
        }
        else if (!Parent) {
            throw std::runtime_error("Code that shouldn't be reached; Node has no grouping ancestor");
        }
        else {
            return Parent->FindGroupingAncestorOrSelf();
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

            if (currentToken.Type == Token::TokenType::Num) {
                // Turn the token's string value into a number
                auto num = std::stod(currentToken.Value);

                // Create a `NumNode` from the number
                auto numNode = std::make_unique<NumNode>(num);

                // Append `numNode` to the focus
                focus->AppendChild(std::move(numNode));
            }
            else if ((int)currentNodeType < (int)focus->Type) {
                if (auto ancestor = focus->FindAncestorOfType(currentNodeType, true)) {
                    // Move the focus to the ancestor of type `currentNodeType`
                    focus = ancestor;

                    // NOTE: The ancestor is the current node
                    // NOTE: We don't need to create another node for the current token
                }
                else {
                    // Create the current node for the current token
                    auto currentNode = std::make_unique<BranchNode>(currentNodeType);

                    // Move the focus to the ancestor
                    focus = focus->FindGroupingAncestorOrSelf();

                    // Insert the current node between the focus and the focus's last child
                    // Then move the focus to the current node
                    InsertNodeBetweenFocusAndLastChildThenMoveFocus(focus, std::move(currentNode));
                }
            }
            else if ((int)currentNodeType > (int)focus->Type) {
                // Create the current node for the current token
                auto currentNode = std::make_unique<BranchNode>(currentNodeType);

                // Insert the current node between the focus and the focus's last child
                // Then move the focus to the current node
                InsertNodeBetweenFocusAndLastChildThenMoveFocus(focus, std::move(currentNode));
            }

            std::cout << "currentToken: " << currentToken.DebugToString() << std::endl;
            std::cout << "exprNode: " << exprNode->DebugToString() << std::endl;
            std::cout << "focus: " << focus->DebugToString() << std::endl;
            std::cout << std::endl;
        }

        return exprNode->DebugToString();
    }
}

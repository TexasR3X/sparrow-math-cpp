// #include <stack>
#include "token_utils.hpp"
#include "expr_utils.hpp"

namespace sparrow_math::internal::parsing_utils {
    void BranchNode::AppendChild(std::unique_ptr<Node> node) {
        node->Parent = this;

        _children.push_back(std::move(node));
    }

    std::unique_ptr<Node> BranchNode::RemoveLastChild() {
        auto lastChild = std::move(_children.back());

        _children.pop_back();

        return std::move(lastChild);
    }

    // BranchNode* BranchNode::FindGroupingAncestor() const {
    //     if (!Parent) {
    //         return nullptr;
    //     }
    //     else if (Parent->Type() == NodeType::Expr || Parent->Type() == NodeType::Delimiters) {
    //         return Parent;
    //     }
    //     else {
    //         return Parent->FindGroupingAncestor();
    //     }
    // }

    BranchNode* BranchNode::FindAncestorOfType(const NodeType& type, const bool& stayWithinDels) const {
        if (!Parent || (stayWithinDels && Parent->Type() == NodeType::Delimiters && type != NodeType::Delimiters)) {
            return nullptr;
        }
        else if (Parent->Type() == type) {
            return Parent;
        }
        else {
            return Parent->FindAncestorOfType(type, stayWithinDels);
        }
    }

    BranchNode* BranchNode::FindAncestorOrSelfOfType(const NodeType& type, const bool& stayWithinDels) {
        return Type() == type ? this : FindAncestorOfType(type, stayWithinDels);
    }

    void BranchNode::InsertParentBetweenSelfAndLastChild(std::unique_ptr<BranchNode> parent) {
        // Remove the last child of the this node
        auto lastChild = RemoveLastChild();

        // Append that child to its new parent node
        parent->AppendChild(std::move(lastChild));

        // Append the new parent to the this node
        AppendChild(std::move(parent));
    }

    // void _AppendChildToFocusThenMoveFocus(BranchNode*& focusNode, std::unique_ptr<BranchNode> child) {
    //     // Store the child node's address as a raw pointer
    //     auto childRawPtr = child.get();

    //     // Make the child node an actual child of the current focused node
    //     focusNode->AppendChild(std::move(child));

    //     // Re-assign the focused node to point to the child
    //     focusNode = childRawPtr;
    // }

    // void _PlaceParentBetweenFocusAndChildThenMoveFocus(BranchNode*& focusNode, std::unique_ptr<BranchNode> newParent) {
    //     // Save the address of the parent as a raw pointer
    //     auto newParentRawPtr = newParent.get();

    //     // Remove the last child of the focused node
    //     auto lastChild = focusNode->RemoveLastChild();

    //     // Append that child to its new parent node
    //     newParent->AppendChild(std::move(lastChild));

    //     // Append the new parent to the focused node
    //     focusNode->AppendChild(std::move(newParent));

    //     // Move the focus to the new parent
    //     focusNode = newParentRawPtr;
    // }

    inline void _HandleNumToken(BranchNode* const focus, const std::string& numStr) {
        auto num = std::stod(numStr);
        auto numNode = std::make_unique<NumNode>(num);

        focus->AppendChild(std::move(numNode));
    }

    inline void _HandlePlusToken(BranchNode*& focus) {
        switch (focus->Type()) {
            case Node::NodeType::Expr:
            case Node::NodeType::Delimiters: {
                // Store a raw pointer to the node that was the focus
                auto oldFocus = focus;

                // Create the sum node
                auto sumNode = std::make_unique<SumNode>();

                // Change the focus to the sum
                focus = sumNode.get();

                // Insert the sum inbetween the old focus and the last appended node
                oldFocus->InsertParentBetweenSelfAndLastChild(std::move(sumNode));
                break;
            }
            case Node::NodeType::Sum:
                // If the next token is a `Plus` and the focused node is a sum, then nothing needs to be done
                break;
            default: {
                // Find the nearest sum ancestor
                auto sumNode = focus->FindAncestorOfType(Node::NodeType::Sum, true);

                // Set the focus to the sum node
                focus = sumNode;
            }
        }
    }

    inline void _HandleStarToken(BranchNode*& focusNode) {
        // switch (focusNode->Type()) {

        // }

        // switch (focusNode->Type()) {
        //     case Node::NodeType::Expr:
        //     case Node::NodeType::Delimiters:
        //     case Node::NodeType::Sum: {
        //         auto productNode = std::make_unique<ProductNode>();
        //         _AppendChildToFocusThenMoveFocus(focusNode, std::move(productNode));
        //         break;
        //     }
        //     case Node::NodeType::Product:
        //         break;
        //     case Node::NodeType::Power: {
        //         auto productNode = std::make_unique<ProductNode>();
        //         _AppendChildToFocusThenMoveFocus(focusNode, std::move(productNode));
        //         break;
        //     }
        //     default:
        //         throw std::runtime_error("Unfinished code reached");
        // }
    }



    std::string ParseExpr(const std::vector<Token>& tokens) {
        auto exprNode = std::make_unique<ExprNode>();

        Token currentToken = Token(Token::TokenType::Unknown);
        Token lastToken = Token(Token::TokenType::Unknown);
        BranchNode* focusNode = exprNode.get();

        for (size_t i = 0; i + 1UL < tokens.size(); ++i) {
            if (i) lastToken = tokens.at(i - 1);
            currentToken = tokens.at(i);

            switch (currentToken.Type) {
                case Token::TokenType::Num: {
                    auto num = std::stod(currentToken.Value);
                    auto numNode = std::make_unique<NumNode>(num);

                    focusNode->AppendChild(std::move(numNode));
                }
                case Token::TokenType::Plus:
                case Token::TokenType::Star:
                case Token::TokenType::UpArrow: {

                }
            }
        }







        // auto exprNode = std::make_unique<ExprNode>();

        // Token currentToken = Token(Token::TokenType::Unknown);
        // Token lastToken = Token(Token::TokenType::Unknown);
        // BranchNode* focusNode = exprNode.get();

        // for (size_t i = 0; i + 1UL < tokens.size(); ++i) {
        //     if (i) {
        //         lastToken = tokens.at(i - 1);
        //     }
        //     currentToken = tokens.at(i);

        //     switch (currentToken.Type) {
        //         case Token::TokenType::Num:
        //             _HandleNumToken(focusNode, currentToken.Value);
        //             break;
        //         case Token::TokenType::Plus:
        //             _HandlePlusToken(focusNode);
        //             break;
        //         case Token::TokenType::Star:
        //             _HandleStarToken(focusNode);
        //             break;
        //         default:
        //             throw std::runtime_error("Unfinished code reached");
        //     }
        // }



        // // DEV NOTE: Temporary Test Code
        // ExprNode exprNode;

        // auto node1 = std::make_unique<NumNode>(1);
        // auto node2 = std::make_unique<NumNode>(2);
        // auto node3 = std::make_unique<NumNode>(3);
        // auto node4 = std::make_unique<NumNode>(4);

        // auto sum = std::make_unique<SumNode>();

        // sum->AppendChild(std::move(node4));

        // exprNode.AppendChild(std::move(node1));
        // exprNode.AppendChild(std::move(node2));
        // exprNode.AppendChild(std::move(sum));
        // exprNode.AppendChild(std::move(node3));

        // return exprNode.DebugToString();

        return "";
    }
}

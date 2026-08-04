#include <cstddef>
#include <format>
#include <iostream>
#include <stdexcept>
#include "token_utils.hpp"
#include "expr_utils.hpp"

namespace sparrow_math::internal::parsing_utils {
    // // ========== Local Functions ========== //

    // Node::NodeType MapTokenTypeToNodeType(Token::TokenType tokenType) {
    //     switch (tokenType) {
    //         case Token::TokenType::DoubleAmpersand:
    //             return Node::NodeType::BooleanAnd;
    //         case Token::TokenType::DoublePipe:
    //             return Node::NodeType::BooleanOr;
    //         case Token::TokenType::Ampersand:
    //             return Node::NodeType::BitwiseAnd;
    //         case Token::TokenType::Pipe:
    //             return Node::NodeType::BitwiseOr;
    //         case Token::TokenType::Plus:
    //             return Node::NodeType::Sum;
    //         case Token::TokenType::Star:
    //             return Node::NodeType::Product;
    //         case Token::TokenType::ForwardSlash:
    //             return Node::NodeType::Fraction;
    //         case Token::TokenType::UpArrow:
    //             return Node::NodeType::Power;
    //         default:
    //             throw std::runtime_error("Code that shouldn't be reached");
    //     }
    // }

    // int GetOrderOfOperationsRank(Token::TokenType tokenType) {
    //     switch (tokenType) {
    //         case Token::TokenType::DoubleAmpersand:
    //         case Token::TokenType::DoublePipe:
    //             return 0;
    //         case Token::TokenType::DoubleEqualSign:
    //         case Token::TokenType::NotEqual:
    //             return 1;
    //         case Token::TokenType::Ampersand:
    //         case Token::TokenType::Pipe:
    //             return 2;
    //         case Token::TokenType::Plus:
    //             return 3;
    //         case Token::TokenType::Star:
    //         case Token::TokenType::ForwardSlash:
    //             return 4;
    //         case Token::TokenType::UpArrow:
    //         case Token::TokenType::Underscore:
    //             return 5;
    //         case Token::TokenType::ExclamationMark:
    //             return 6;
    //         case Token::TokenType::LeftParenthesis:
    //         case Token::TokenType::LeftSquareBracket:
    //         case Token::TokenType::RightSquareBracket:
    //         case Token::TokenType::LeftCurlyBracket:
    //         case Token::TokenType::RightCurlyBracket:
    //         case Token::TokenType::EscapedLeftCurlyBracket:
    //         case Token::TokenType::EscapedRightCurlyBracket:
    //         case Token::TokenType::LeftAngleBracket:
    //         case Token::TokenType::RightAngleBracket:
    //             return 7;
    //         default:
    //             throw std::runtime_error("Unfinished code in `MapTokenTypeToNodeType()`");
    //     }
    // }

    // // std::unordered_map<Token::TokenType, int> orderOfOperationsMap = {
    // //     { Token::TokenType::DoubleAmpersand, 0 },
    // //     { Token::TokenType::DoublePipe, 0 },
    // //     { Token::TokenType::DoubleEqualSign, 1 },
    // //     { Token::TokenType::NotEqual, 1 },
    // //     { Token::TokenType::Ampersand, 2 },
    // //     { Token::TokenType::Pipe, 2 },
    // //     { Token::TokenType::Plus, 3 },
    // //     { Token::TokenType::Star, 4 },
    // //     { Token::TokenType::ForwardSlash, 4 },
    // //     { Token::TokenType::UpArrow, 5 },
    // //     { Token::TokenType::Underscore, 5 },
    // //     { Token::TokenType::ExclamationMark, 6 },
    // //     { Token::TokenType::LeftParenthesis, 7 },
    // //     { Token::TokenType::LeftSquareBracket, 7 },
    // //     { Token::TokenType::RightSquareBracket, 7 },
    // //     { Token::TokenType::LeftCurlyBracket, 7 },
    // //     { Token::TokenType::RightCurlyBracket, 7 },
    // //     { Token::TokenType::EscapedLeftCurlyBracket, 7 },
    // //     { Token::TokenType::EscapedRightCurlyBracket, 7 },
    // //     { Token::TokenType::LeftAngleBracket, 7 },
    // //     { Token::TokenType::RightAngleBracket, 7 }
    // // };

    // // Sum,
    // // Product,
    // // Power,
    // // DelimiterGrouping,
    // // Expr,
    // // Symbol,
    // // Num

    // // ForwardSlash,
    // // Underscore,
    // // UpArrow,
    // // Ampersand,
    // // DoubleAmpersand,
    // // Pipe,
    // // DoublePipe,
    // // EqualSign,
    // // DoubleEqualSign,
    // // ExclamationMark,
    // // NotEqual,
    // // LineBreak,

    // // LeftParenthesis,
    // // RightParenthesis,
    // // LeftSquareBracket,
    // // RightSquareBracket,
    // // LeftCurlyBracket,
    // // RightCurlyBracket,
    // // EscapedLeftCurlyBracket,
    // // EscapedRightCurlyBracket,
    // // LeftAngleBracket,
    // // RightAngleBracket,

    // Create a new node inbetween the focus and its last child. Then move the focus to
    // the new node (reassign the variable `focus` to store the address of the new node).
    void InsertNodeBetweenFocusAndLastChildThenMoveFocus(BranchNode*& focus, Operator nodeOp) {
        // Create the new node
        auto newNode = std::make_unique<OperatorNode>(nodeOp);

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

    BranchNode* BranchNode::GetNearestAncestorOfType(const Operator& type, bool stayWithinDels) const {
        auto parent = Parent->CastAsType<OperatorNode>();

        if (!parent || (stayWithinDels && parent->Op == Operator::OperatorType::DelimiterGrouping && type != Operator::OperatorType::DelimiterGrouping)) {
            return nullptr;
        }
        else if (parent->Op == type) {
            return parent;
        }
        else {
            return Parent->GetNearestAncestorOfType(type, stayWithinDels);
        }
    }

    // BranchNode* BranchNode::GetNearestAncestorOfType(NodeType type, bool stayWithinDels) const {
    //     if (!Parent || (stayWithinDels && Parent->Type == NodeType::DelimiterGrouping && type != NodeType::DelimiterGrouping)) {
    //         return nullptr;
    //     }
    //     else if (Parent->Type == type) {
    //         return Parent;
    //     }
    //     else {
    //         return Parent->GetNearestAncestorOfType(type, stayWithinDels);
    //     }
    // }

    BranchNode* BranchNode::GetNearestAncestorOrSelfOfType(const Operator& type, bool stayWithinDels) {
        return Type == NodeType::OperatorNode && CastAsType<OperatorNode>()->Op == type ? this : GetNearestAncestorOfType(type, stayWithinDels);
    }

    // BranchNode* BranchNode::GetNearestAncestorOrSelfOfType(NodeType type, bool stayWithinDels) {
    //     return Type == type ? this : GetNearestAncestorOfType(type, stayWithinDels);
    // }

    BranchNode* BranchNode::GetNearestGroupingAncestorOrSelf() {
        if (Type == NodeType::Expr || (Type == NodeType::OperatorNode && Parent->CastAsType<OperatorNode>()->Op == Operator::OperatorType::DelimiterGrouping)) {
            return this;
        }
        else if (!Parent) {
            throw std::runtime_error("Code that shouldn't be reached; Node has no grouping ancestor");
        }
        else {
            return Parent->GetNearestGroupingAncestorOrSelf();
        }
    }

    std::string BranchNode::DebugToString() {
        std::string name;
        std::string childrenStr;

        if (auto exprNode = CastAsType<ExprNode>()) {
            name = "Expr";
        }
        else if (auto opNode = CastAsType<OperatorNode>()) {
            name = opNode->Op.DebugToString();
        }
        else {
            throw std::runtime_error("Code that shouldn't be reached");
        }

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
            std::cout << "[BEFORE]" << std::endl;

            if (currentToken.Type == Token::TokenType::Num) {
                std::cout << "[currentToken.Type == Token::TokenType::Num]" << std::endl;

                // Turn the token's string value into a number
                auto num = std::stod(currentToken.Value);

                // Create a `NumNode` from the number
                auto numNode = std::make_unique<NumNode>(num);

                // Append `numNode` to the focus
                focus->AppendChild(std::move(numNode));
            }
            else if (currentToken.Type == Token::TokenType::Symbol) {
                std::cout << "[currentToken.Type == Token::TokenType::Symbol]" << std::endl;

                // Create a `SymbolNode` from the symbol token
                auto symbolNode = std::make_unique<SymbolNode>(currentToken.Value);

                // Append `symbolNode` to the focus
                focus->AppendChild(std::move(symbolNode));
            }
            else if (currentToken.Type == Token::TokenType::Underscore) {
                std::cout << "[currentToken.Type == Token::TokenType::Underscore]" << std::endl;

                throw std::runtime_error("Unfinished code");
            }
            else if (currentToken.Type == Token::TokenType::ExclamationMark) {
                std::cout << "[currentToken.Type == Token::TokenType::ExclamationMark]" << std::endl;

                throw std::runtime_error("Unfinished code");
            }
            else if (currentToken.IsTokenLeftDelimiter()) {
                std::cout << "[currentToken.IsTokenLeftDelimiter()]" << std::endl;

                InsertNodeBetweenFocusAndLastChildThenMoveFocus(focus, Operator::OperatorType::DelimiterGrouping);
            }
            else if (currentToken.IsTokenRightDelimiter()) {
                std::cout << "[currentToken.IsTokenRightDelimiter()]" << std::endl;

                // Get the parent of the nearest delimiter ancestor
                auto delParent = focus->GetNearestGroupingAncestorOrSelf()->Parent;

                // Get the nearest delimiter and remove it from its parent
                auto del = delParent->RemoveLastChild();

                if (auto delRawPtr = del->CastAsType<BranchNode>()) {
                    // Remove the child from the delimiter
                    auto delChild = delRawPtr->RemoveLastChild();

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
                std::cout << "[ELSE]" << std::endl;

                // auto orderOfOperationsRank = GetOrderOfOperationsRank(currentToken.Type);
                // auto currentNodeType = MapTokenTypeToNodeType(currentToken.Type);

                // std::cout << "orderOfOperationsRank: " << orderOfOperationsRank << std::endl;
                // std::cout << "(int)focus->Type: " << (int)focus->Type << std::endl;


                // const Operator currentTokenOp(currentToken.Type);
                // const auto& focusOp = focus->CastAsType<OperatorNode>()->Op;
                // std::cout << "focusOp: " << focusOp.DebugToString() << std::endl;

                const Operator currentTokenOp(currentToken.Type);
                const auto focusNodeAsOp = focus->CastAsType<OperatorNode>();
                const auto& focusOp = focusNodeAsOp ? focusNodeAsOp->Op : Operator::OperatorType::Expr;

                if (currentTokenOp > focusOp) {
                    std::cout << "[currentTokenOp > focusOp]" << std::endl;

                    InsertNodeBetweenFocusAndLastChildThenMoveFocus(focus, currentTokenOp);
                }
                else if (currentTokenOp < focusOp) {
                    std::cout << "[currentTokenOp < focusOp]" << std::endl;

                    if (auto ancestor = focus->GetNearestAncestorOfType(currentTokenOp, true)) {
                        // Move the focus to the ancestor of type `currentNodeType`
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

        return exprNode->DebugToString();
    }
}

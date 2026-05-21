#include <stack>
#include "token_utils.hpp"
#include "expr_utils.hpp"

namespace sparrow_math::internal::parsing_utils {
    void BranchNode::PushChild(std::unique_ptr<Node> node) {
        Parent = node.get();

        _children.push_back(std::move(node));
    }

    std::string ParseExpr(std::vector<Token> tokens) {
        // DEV NOTE: Temporary Test Code
        ExprNode exprNode;

        auto node1 = std::make_unique<NumNode>(1);
        auto node2 = std::make_unique<NumNode>(2);
        auto node3 = std::make_unique<NumNode>(3);
        auto node4 = std::make_unique<NumNode>(4);

        auto sum = std::make_unique<SumNode>();

        sum->PushChild(std::move(node4));

        exprNode.PushChild(std::move(node1));
        exprNode.PushChild(std::move(node2));
        exprNode.PushChild(std::move(sum));
        exprNode.PushChild(std::move(node3));

        return exprNode.DebugToString();

        // for (auto token : tokens) {
        //     switch (token.Type) {
        //         case Token::TokenType::Number:
        //             long long num = std::stoll(token.Value);

        //             auto node = std::make_unique<NumNode>(num);

        //             exprNode.PushChild(std::move(node));

        //             break;
        //         case Token::TokenType::Plus:
        //     }
        // }
    }
}












// namespace sparrow_math::internal::parsing_utils {
//     Token TokenIterator::MoveForward() {
//         Token token = Peek();

//         ++_index;

//         return token;
//     }

//     Token TokenIterator::MoveBackward() {
//         Token token = Peek();

//         --_index;

//         return token;
//     }

//     void ParseExpr(std::vector<Token> tokens) {
//         TokenIterator it(tokens);

//         int loopCount = 0;
//         while (loopCount < 9e9) {

//         }
//     }
// }












// namespace sparrow_math::internal::parsing_utils {
//     enum class DelimiterType {
//         LeftParenthesis,
//         RightParenthesis,
//         LeftSquareBracket,
//         RightSquareBracket,
//         LeftCurlyBracket,
//         RightCurlyBracket,
//         EscapedLeftCurlyBracket,
//         EscapedRightCurlyBracket
//     };

//     void EnsureDelimitersAreBalanced(std::vector<Token>& tokens) {
//         std::stack<DelimiterType> delStack;

//         for (auto token : tokens) {
            
//         }
//     }

//     std::vector<ExprNode> ParseExpr(std::vector<Token>& tokens) {
//         static std::vector<ExprNode::NodeType> opTypes = {
//             ExprNode::NodeType::Sum,
//             ExprNode::NodeType::Product,
//             ExprNode::NodeType::Exponent
//         };
//         // DelimiterManager delManager;

//         for (auto type : opTypes) {
//             for (auto token : tokens) {

//             }
//         }
//     }
// }

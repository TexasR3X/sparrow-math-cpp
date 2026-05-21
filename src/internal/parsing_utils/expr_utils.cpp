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
    }
}

//
// Created by Andrew Nazareth on 3/27/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_CLOSEDTREENODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_CLOSEDTREENODE_HPP
#include "LeafTreeNode.hpp"

namespace Logic_Project {
class ClosedTreeNode final : public LeafTreeNode {
    public:
    ClosedTreeNode();

    std::any Accept(TreeVisitor& visitor) const override;
    bool AddNode(std::unique_ptr<TreeNode> node) override;
    std::unique_ptr<LogicExpression> GetStatement() override;
    bool SetAntecedent(int id) override
    {
        return false;
    }

    int decomposition1;
    int decomposition2;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_CLOSEDTREENODE_HPP

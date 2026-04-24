//
// Created by Andrew Nazareth on 3/27/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_OPENTREENODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_OPENTREENODE_HPP
#include "LeafTreeNode.hpp"
namespace Logic_Project {
class OpenTreeNode final : public LeafTreeNode {
    public:
    OpenTreeNode();

    bool AddNode(std::unique_ptr<TreeNode> node) override;
    std::any Accept(TreeVisitor& visitor) const override;
    std::unique_ptr<LogicExpression> GetStatement() override;
    bool SetAntecedent(int id) override
    {
        return false;
    }
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_OPENTREENODE_HPP

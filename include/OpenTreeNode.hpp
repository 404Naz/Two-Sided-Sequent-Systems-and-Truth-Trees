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

    std::any Accept(TreeVisitor& visitor) override;
    [[nodiscard]] std::unique_ptr<TreeNode> Copy() const override;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_OPENTREENODE_HPP

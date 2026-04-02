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

    std::any Accept(TreeVisitor& visitor) override;
    [[nodiscard]] std::unique_ptr<TreeNode> Copy() const override;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_CLOSEDTREENODE_HPP

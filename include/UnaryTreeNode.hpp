//
// Created by Andrew Nazareth on 3/27/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_UNARYTREENODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_UNARYTREENODE_HPP
#include "TreeNode.hpp"

namespace Logic_Project {
class UnaryTreeNode final : public TreeNode {
    public:
    UnaryTreeNode();

    std::unique_ptr<TreeNode> child;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_UNARYTREENODE_HPP

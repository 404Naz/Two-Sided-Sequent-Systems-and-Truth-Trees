//
// Created by Andrew Nazareth on 3/27/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_BINARYTREENODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_BINARYTREENODE_HPP

#include "TreeNode.hpp"

namespace Logic_Project {

class BinaryTreeNode final : public TreeNode {
    public:
    BinaryTreeNode() = default;

    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_BINARYTREENODE_HPP

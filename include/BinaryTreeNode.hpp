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
    BinaryTreeNode(const BinaryTreeNode& other);
    explicit BinaryTreeNode(const TreeNode& parent);
    explicit BinaryTreeNode(const LogicExpression& statement);
    BinaryTreeNode(const LogicExpression& statement, const TreeNode& parent);
    BinaryTreeNode(const LogicExpression& statement, const TreeNode& parent, const TreeNode& left, const TreeNode& right);

    std::any Accept(TreeVisitor& visitor) const override;

    [[nodiscard]] std::unique_ptr<TreeNode> Copy() const override;

    [[nodiscard]] bool HasLeftChild() const;
    [[nodiscard]] bool HasRightChild() const;
    void SetLeftChild(const TreeNode& leftChild);
    void SetRightChild(const TreeNode& rightChild);

    // Statement represented by this node
    std::unique_ptr<LogicExpression> statement;
    std::unique_ptr<TreeNode> left;
    std::unique_ptr<TreeNode> right;
};

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_BINARYTREENODE_HPP

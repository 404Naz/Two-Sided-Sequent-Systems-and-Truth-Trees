//
// Created by Andrew Nazareth on 3/31/26.
//

#include "BinaryTreeNode.hpp"
#include <deque>

namespace Logic_Project {

BinaryTreeNode::BinaryTreeNode(const LogicExpression& statement, const TreeNode& parent, const TreeNode& left, const TreeNode& right)
{
    this->statement = statement.Copy();
    this->parent = parent.Copy();
    SetLeftChild(left);
    SetRightChild(right);
}

BinaryTreeNode::BinaryTreeNode(const LogicExpression& statement)
{
    this->statement = statement.Copy();
}

BinaryTreeNode::BinaryTreeNode(const TreeNode& parent)
{
    SetParent(parent);
}

BinaryTreeNode::BinaryTreeNode(const LogicExpression& statement, const TreeNode& parent)
{
    this->statement = statement.Copy();
    SetParent(parent);
}
bool BinaryTreeNode::HasLeftChild() const
{
    return this->left != nullptr;
}

bool BinaryTreeNode::HasRightChild() const
{
    return this->right != nullptr;
}

void BinaryTreeNode::SetLeftChild(const TreeNode& leftChild)
{
    this->left = leftChild.Copy();
}
void BinaryTreeNode::SetRightChild(const TreeNode& rightChild)
{
    this->right = rightChild.Copy();
}

BinaryTreeNode::BinaryTreeNode(const BinaryTreeNode& other)
 : TreeNode(other)
{
    SetLeftChild(*other.left);
    SetRightChild(*other.right);
    SetParent(*other.parent);
    this->statement = other.statement->Copy();
}

std::any BinaryTreeNode::Accept(TreeVisitor& visitor) const
{
    return visitor.Visit(*this);
}
std::unique_ptr<TreeNode> BinaryTreeNode::Copy() const
{
    return std::make_unique<BinaryTreeNode>(*static_cast<const BinaryTreeNode*>(this));
}

}
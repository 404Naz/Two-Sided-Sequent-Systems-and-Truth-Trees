//
// Created by Andrew Nazareth on 3/31/26.
//

#include "BinaryTreeNode.hpp"
#include <deque>

namespace Logic_Project {

BinaryTreeNode::BinaryTreeNode(const LogicExpression& statement, TreeNode& parent, std::unique_ptr<TreeNode> left, std::unique_ptr<TreeNode> right)
{
    this->statement = statement.Copy();
    this->parent = &parent;
    SetLeftChild(std::move(left));
    SetRightChild(std::move(right));
    this->decomposition1 = 0;
    this->isPremise = false;
}

BinaryTreeNode::BinaryTreeNode(const LogicExpression& statement)
{
    this->statement = statement.Copy();
    this->decomposition1 = 0;
    this->isPremise = false;
}

BinaryTreeNode::BinaryTreeNode(TreeNode& parent)
{
    SetParent(parent);
    this->decomposition1 = 0;
    this->isPremise = false;
}

BinaryTreeNode::BinaryTreeNode(const LogicExpression& statement, TreeNode& parent)
{
    this->statement = statement.Copy();
    SetParent(parent);
    this->decomposition1 = 0;
    this->isPremise = false;
}
bool BinaryTreeNode::HasLeftChild() const
{
    return this->left != nullptr;
}

bool BinaryTreeNode::HasRightChild() const
{
    return this->right != nullptr;
}

void BinaryTreeNode::SetLeftChild(std::unique_ptr<TreeNode> leftChild)
{
    this->left = std::move(leftChild);
    if (this->left != nullptr) this->left->SetParent(*this);
}
void BinaryTreeNode::SetRightChild(std::unique_ptr<TreeNode> rightChild)
{
    this->right = std::move(rightChild);
    if (this->right != nullptr) this->right->SetParent(*this);
}

BinaryTreeNode::BinaryTreeNode(const BinaryTreeNode& other)
 : TreeNode(other)
{
    if (other.left) SetLeftChild(other.left->Copy());
    if (other.right) SetRightChild(other.right->Copy());
    this->statement = other.statement->Copy();
    this->decomposition1 = 0;
    this->isPremise = false;
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
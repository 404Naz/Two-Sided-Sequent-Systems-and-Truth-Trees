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
    this->antecedent = -1;
    this->isPremise = false;
}

BinaryTreeNode::BinaryTreeNode(const LogicExpression& statement)
{
    this->statement = statement.Copy();
    this->antecedent = -1;
    this->isPremise = false;
}

BinaryTreeNode::BinaryTreeNode(TreeNode& parent)
{
    SetParent(parent);
    this->antecedent = -1;
    this->isPremise = false;
}

BinaryTreeNode::BinaryTreeNode(const LogicExpression& statement, TreeNode& parent)
{
    this->statement = statement.Copy();
    SetParent(parent);
    this->antecedent = -1;
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

std::any BinaryTreeNode::Accept(TreeVisitor& visitor) const
{
    return visitor.Visit(*this);
}
bool BinaryTreeNode::AddNode(std::unique_ptr<TreeNode> node)
{
    if (this->left == nullptr) {
        this->SetLeftChild(std::move(node));
        return true;
    }
    if (this->left != nullptr && this->right == nullptr) {
        this->SetRightChild(std::move(node));
        return true;
    }
    return false;
}
std::unique_ptr<LogicExpression> BinaryTreeNode::GetStatement()
{
    return statement->Copy();
}

}
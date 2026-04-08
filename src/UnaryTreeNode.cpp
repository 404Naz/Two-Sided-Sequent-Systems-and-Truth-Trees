//
// Created by Andrew Nazareth on 4/2/26.
//

#include "UnaryTreeNode.hpp"

namespace Logic_Project {
UnaryTreeNode::UnaryTreeNode(const UnaryTreeNode& other)
 : TreeNode(other)
{
    if (other.child != nullptr) this->child = other.child->Copy();
    decomposition1 = other.decomposition1;
    isPremise = other.isPremise;
}

UnaryTreeNode::UnaryTreeNode(const LogicExpression& statement)
{
    this->statement = statement.Copy();
    decomposition1 = 0;
    isPremise = false;
}

UnaryTreeNode::UnaryTreeNode(const LogicExpression& statement, TreeNode& parent)
{
    this->statement = statement.Copy();
    this->parent = &parent;
    decomposition1 = 0;
    isPremise = false;
}

UnaryTreeNode::UnaryTreeNode(const LogicExpression& statement, TreeNode& parent, std::unique_ptr<TreeNode> child)
{
    this->statement = statement.Copy();
    this->parent = &parent;
    this->child = std::move(child);
    decomposition1 = 0;
    isPremise = false;
}

std::any UnaryTreeNode::Accept(TreeVisitor& visitor) const
{
    return visitor.Visit(*this);
}

std::unique_ptr<TreeNode> UnaryTreeNode::Copy() const
{
    return std::make_unique<UnaryTreeNode>(*static_cast<const UnaryTreeNode*>(this));
}

bool UnaryTreeNode::HasChild() const
{
    return this->child != nullptr;
}

void UnaryTreeNode::SetChild(std::unique_ptr<TreeNode> newChild)
{
    this->child = std::move(newChild);
    if (this->child != nullptr) this->child->SetParent(*this);
}

}
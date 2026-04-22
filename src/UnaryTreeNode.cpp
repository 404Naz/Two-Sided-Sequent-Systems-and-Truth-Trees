//
// Created by Andrew Nazareth on 4/2/26.
//

#include "UnaryTreeNode.hpp"

namespace Logic_Project {
UnaryTreeNode::UnaryTreeNode(const LogicExpression& statement)
{
    this->statement = statement.Copy();
    antecedent = -1;
    isPremise = false;
}

UnaryTreeNode::UnaryTreeNode(const LogicExpression& statement, TreeNode& parent)
{
    this->statement = statement.Copy();
    this->parent = &parent;
    antecedent = -1;
    isPremise = false;
}

UnaryTreeNode::UnaryTreeNode(const LogicExpression& statement, TreeNode& parent, std::unique_ptr<TreeNode> child)
{
    this->statement = statement.Copy();
    this->parent = &parent;
    this->child = std::move(child);
    antecedent = -1;
    isPremise = false;
}

std::any UnaryTreeNode::Accept(TreeVisitor& visitor) const
{
    return visitor.Visit(*this);
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
bool UnaryTreeNode::AddNode(std::unique_ptr<TreeNode> node)
{
    if (this->child == nullptr) {
        this->SetChild(std::move(node));
        return true;
    }
    return false;
}
std::unique_ptr<LogicExpression> UnaryTreeNode::GetStatement()
{
    return statement->Copy();
}
}
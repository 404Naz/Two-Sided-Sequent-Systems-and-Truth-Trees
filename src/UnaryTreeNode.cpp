//
// Created by Andrew Nazareth on 4/2/26.
//

#include "UnaryTreeNode.hpp"

namespace Logic_Project {
UnaryTreeNode::UnaryTreeNode(const UnaryTreeNode& other)
 : TreeNode(other)
{
}

UnaryTreeNode::UnaryTreeNode(const LogicExpression& statement)
{
}

UnaryTreeNode::UnaryTreeNode(const LogicExpression& statement, const TreeNode& parent)
{
}

UnaryTreeNode::UnaryTreeNode(const LogicExpression& statement, const TreeNode& parent, const TreeNode& child)
{
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

void UnaryTreeNode::SetChild(const TreeNode& newChild)
{
    this->child = newChild.Copy();
}

}
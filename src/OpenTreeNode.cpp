//
// Created by Andrew Nazareth on 4/2/26.
//

#include "OpenTreeNode.hpp"

namespace Logic_Project {
OpenTreeNode::OpenTreeNode()
    : LeafTreeNode()
{
}

std::any OpenTreeNode::Accept(TreeVisitor& visitor) const
{
    return visitor.Visit(*this);
}
bool OpenTreeNode::AddNode(std::unique_ptr<TreeNode>)
{
    return false;
}
std::unique_ptr<LogicExpression> OpenTreeNode::GetStatement()
{
    return nullptr;
}

}
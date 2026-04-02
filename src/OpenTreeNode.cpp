//
// Created by Andrew Nazareth on 4/2/26.
//

#include "OpenTreeNode.hpp"

namespace Logic_Project {
OpenTreeNode::OpenTreeNode()
    : LeafTreeNode()
{
}

std::any OpenTreeNode::Accept(TreeVisitor& visitor)
{
    return visitor.Visit(*this);
}

std::unique_ptr<TreeNode> OpenTreeNode::Copy() const
{
    return std::make_unique<OpenTreeNode>();
}

}
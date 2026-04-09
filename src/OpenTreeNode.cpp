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

}
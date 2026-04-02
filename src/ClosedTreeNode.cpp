//
// Created by Andrew Nazareth on 4/2/26.
//

#include "ClosedTreeNode.hpp"

namespace Logic_Project {
 ClosedTreeNode::ClosedTreeNode()
     : LeafTreeNode()
{
}

std::any ClosedTreeNode::Accept(TreeVisitor& visitor)
{
     return visitor.Visit(*this);
}

std::unique_ptr<TreeNode> ClosedTreeNode::Copy() const
{
     return std::make_unique<ClosedTreeNode>();
}

}
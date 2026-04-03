//
// Created by Andrew Nazareth on 4/2/26.
//

#include "ClosedTreeNode.hpp"

namespace Logic_Project {
 ClosedTreeNode::ClosedTreeNode()
     : LeafTreeNode()
{
     decomposition1 = 0;
     decomposition2 = 0;
}

std::any ClosedTreeNode::Accept(TreeVisitor& visitor) const
{
     return visitor.Visit(*this);
}

std::unique_ptr<TreeNode> ClosedTreeNode::Copy() const
{
     return std::make_unique<ClosedTreeNode>();
}

}
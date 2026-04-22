//
// Created by Andrew Nazareth on 4/2/26.
//

#include "ClosedTreeNode.hpp"

#include "LogicalContradiction.hpp"

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
bool ClosedTreeNode::AddNode(std::unique_ptr<TreeNode>)
{
     return false;
}
std::unique_ptr<LogicExpression> ClosedTreeNode::GetStatement()
{
     return LogicalContradiction{}.Generalize();
}

}
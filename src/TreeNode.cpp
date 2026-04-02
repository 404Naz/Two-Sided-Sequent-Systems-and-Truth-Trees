//
// Created by Andrew Nazareth on 4/2/26.
//

#include "TreeNode.hpp"

namespace Logic_Project {
int TreeNode::globalId = 0;

TreeNode::TreeNode()
{
     id = ++globalId;
}
TreeNode::TreeNode(const TreeNode& node)
{
     id = ++globalId;
}
void TreeNode::SetParent(const TreeNode& node)
{
     this->parent = node.Copy();
}
bool TreeNode::HasParent() const
{
     return this->parent != nullptr;
}

int TreeNode::GetId() const
{
    return id;
}

}
//
// Created by Andrew Nazareth on 4/2/26.
//

#include "TreeNode.hpp"

namespace Logic_Project {
int TreeNode::globalId = 0;

TreeNode::TreeNode()
{
    id = globalId++;
    parent = nullptr;
}
TreeNode::TreeNode(const TreeNode& node)
{
    id = globalId++;
    parent = nullptr;
}
void TreeNode::SetParent(TreeNode& node)
{
     this->parent = &node;
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
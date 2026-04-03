//
// Created by Andrew Nazareth on 4/2/26.
//

#include "TreeSerializer.hpp"

#include "ClosedTreeNode.hpp"
#include "OpenTreeNode.hpp"
#include "BinaryTreeNode.hpp"
#include "UnaryTreeNode.hpp"

#include <format>

namespace Logic_Project {

std::any TreeSerializer::Visit(const UnaryTreeNode& node)
{
    return std::string{"Not Implemented"};
}

std::any TreeSerializer::Visit(const BinaryTreeNode& node)
{
    return std::string{"Not Implemented"};
}

std::any TreeSerializer::Visit(const ClosedTreeNode& node)
{
    // {"id":6,"text":"×","children":[],"decomposition":[4,2],"parent":4}
    std::string decomposition = std::format("\"id\":{},\"text\":\"×\",\"children\":[],\"decomposition\":[{},{}],\"parent\":{}", \
            node.GetId(), node.decomposition1, node.decomposition2, node.parent->GetId());

    serializedNodes.push_back(decomposition);

    return decomposition;
}

std::any TreeSerializer::Visit(const OpenTreeNode& node)
{
    // {"id":6,"text":"◯","children":[],"decomposition":[],"parent":4}
    std::string decomposition = std::format("\"id\":{},\"text\":\"◯\",\"children\":[],\"decomposition\":[],\"parent\":{}",\
            node.GetId(), node.parent->GetId());

    serializedNodes.push_back(decomposition);

    return decomposition;
}
std::string TreeSerializer::Serialize()
{
    return std::string{"Not Implemented"};
}
std::string TreeSerializer::Serialize(const TreeNode& root)
{
    root.Accept(*this);
    return this->Serialize();
}
}
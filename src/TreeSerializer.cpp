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
    std::string childText, decompositionText, parentText, antecedentText;
    if (node.antecedent >= 0) {
        antecedentText = std::format(R"("antecedent":{},)", node.antecedent);
    }
    if (node.decomposition1 >= 0 && node.decomposition2 >= 0) {
        decompositionText = std::format(R"({},{})", node.decomposition1, node.decomposition2);
    }
    if (node.HasChild())
    {
        childText = std::to_string(node.child->GetId());
    }
    if (node.HasParent()) {
        parentText = std::format(R"("parent":{},)", node.parent->GetId());
    }

    std::string decomposition = std::format(R"("id":{},"text":"{}","children":[{}],"decomposition":[{}],{}{}"premise":{})", \
            node.GetId(), node.statement->Serialize(), childText, decompositionText, parentText, antecedentText, node.isPremise);

    serializedNodes.push_back("{"+decomposition+"}");

    if (node.HasChild()) {
        auto childDecomp = std::any_cast<std::string>(node.child->Accept(*this));
        decomposition.append(","+childDecomp);
    }

    return decomposition;
}

std::any TreeSerializer::Visit(const BinaryTreeNode& node)
{
    std::string childText, decompositionText, parentText, antecedentText;
    if (node.antecedent >= 0) {
        antecedentText = std::format(R"("antecedent":{},)", node.antecedent);
    }
    if (node.decomposition1 >= 0 && node.decomposition2 >= 0) {
        decompositionText = std::format(R"({},{})", node.decomposition1, node.decomposition2);
    }
    if (node.HasLeftChild() && node.HasRightChild())
    {
        childText = std::format("{},{}", node.left->GetId(), node.right->GetId());
    }
    if (node.HasParent()) {
        parentText = std::format(R"("parent":{},)", node.parent->GetId());
    }
    std::string decomposition = std::format(R"("id":{},"text":"{}","children":[{}],"decomposition":[{}],{}{}"premise":{})", \
            node.GetId(), node.statement->Serialize(), childText, decompositionText, parentText, antecedentText, node.isPremise);

    serializedNodes.push_back("{"+decomposition+"}");

    if (node.HasLeftChild()) decomposition.append(","+std::any_cast<std::string>(node.left->Accept(*this)));
    if (node.HasRightChild()) decomposition.append(","+std::any_cast<std::string>(node.right->Accept(*this)));

    return decomposition;
}

std::any TreeSerializer::Visit(const ClosedTreeNode& node)
{
    // {"id":6,"text":"×","children":[],"decomposition":[4,2],"parent":4}
    std::string decomposition = std::format("\"id\":{},\"text\":\"×\",\"children\":[],\"decomposition\":[{},{}],\"parent\":{}", \
            node.GetId(), node.decomposition1, node.decomposition2, node.parent->GetId());

    serializedNodes.push_back("{"+decomposition+"}");

    return decomposition;
}

std::any TreeSerializer::Visit(const OpenTreeNode& node)
{
    // {"id":6,"text":"◯","children":[],"decomposition":[],"parent":4}
    std::string decomposition = std::format("\"id\":{},\"text\":\"◯\",\"children\":[],\"decomposition\":[],\"parent\":{}",\
            node.GetId(), node.parent->GetId());

    serializedNodes.push_back("{"+decomposition+"}");

    return decomposition;
}

std::string TreeSerializer::SerializeNodes() const
{
    if (serializedNodes.empty()) {
        return "";
    }
    std::string serialized = R"("nodes":[)";
    for (size_t i = 0; i < serializedNodes.size()-1; i++) {
        serialized.append(serializedNodes[i]);
        serialized.append(",");
    }
    serialized.append(serializedNodes[serializedNodes.size()-1]);
    serialized.append("],");
    return serialized;
}

std::string TreeSerializer::Serialize() const
{
    std::string serialized = "{";

    serialized.append(SerializeNodes());

    serialized.append(R"("options":{"requireAtomicContradiction":true,"requireAllBranchesTerminated":true,"lockedOptions":false}})");
    return serialized;
}
std::string TreeSerializer::Serialize(const TreeNode& root)
{
    serializedNodes.clear();
    root.Accept(*this);
    return this->Serialize();
}
}
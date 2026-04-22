//
// Created by Andrew Nazareth on 3/31/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTCONVERTER_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTCONVERTER_HPP

#include "SequentVisitor.hpp"
#include "SequentNode.hpp"
#include "TreeNode.hpp"
#include <any>
#include <vector>
#include <string>

namespace Logic_Project {

class SequentConverter final : public SequentVisitor {
    public:
    SequentConverter() = default;

    std::any Visit(const UnarySequentNode& node) override;
    std::any Visit(const BinarySequentNode& node) override;
    std::unique_ptr<TreeNode> ConvertToTree(SequentNode& node);

    std::vector<std::unique_ptr<TreeNode>> createdNodes;
    std::vector<int> createdNodesParentIds;
    std::vector<int> createdNodesDecompositions1;
    std::vector<int> createdNodesDecompositions2;
};

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTCONVERTER_HPP

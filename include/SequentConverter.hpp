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

    std::any VisitOld(const UnarySequentNode& node);
    std::any VisitOld(const BinarySequentNode& node);
    std::unique_ptr<TreeNode> Visit(const UnarySequentNode& unary) override;
    std::unique_ptr<TreeNode> Visit(const BinarySequentNode& binary) override;
    std::unique_ptr<TreeNode> ConvertToTree(SequentNode& node);

    std::vector<std::unique_ptr<LogicExpression>> premises;

    std::vector<std::unique_ptr<TreeNode>> createdNodes;
    std::vector<int> createdNodesParentIds;
    std::vector<int> createdNodesDecompositions1;
    std::vector<int> createdNodesDecompositions2;
};

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTCONVERTER_HPP

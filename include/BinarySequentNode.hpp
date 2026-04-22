//
// Created by Andrew Nazareth on 3/31/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_BINARYSEQUENTNODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_BINARYSEQUENTNODE_HPP

#include "SequentNode.hpp"
#include "LogicExpression.hpp"

namespace Logic_Project {
class BinarySequentNode final: public SequentNode {
public:
    BinarySequentNode() = delete;
    BinarySequentNode(const std::vector<std::unique_ptr<LogicExpression>>& antecedents, const std::vector<std::unique_ptr<LogicExpression>>& succedents);
    BinarySequentNode(const std::vector<std::unique_ptr<LogicExpression>>& antecedents, const std::vector<std::unique_ptr<LogicExpression>>& succedents, const SequentNodeRule& rule);

    void SetLeftParent(SequentNode* left);
    void SetRightParent(SequentNode* right);

    std::any Accept(SequentVisitor& visitor) override;

    SequentNode* leftParent = nullptr;
    SequentNode* rightParent = nullptr;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_BINARYSEQUENTNODE_HPP

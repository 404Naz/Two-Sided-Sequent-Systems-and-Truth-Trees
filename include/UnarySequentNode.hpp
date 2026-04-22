//
// Created by Andrew Nazareth on 3/31/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_UNARYSEQUENTNODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_UNARYSEQUENTNODE_HPP

#include "SequentNode.hpp"

namespace Logic_Project {
class UnarySequentNode final : public SequentNode {
    public:
    UnarySequentNode() = delete;
    UnarySequentNode(const std::vector<std::unique_ptr<LogicExpression>>& antecedents, const std::vector<std::unique_ptr<LogicExpression>>& succedents);
    UnarySequentNode(const std::vector<std::unique_ptr<LogicExpression>>& antecedents, const std::vector<std::unique_ptr<LogicExpression>>& succedents, const SequentNodeRule& rule);

    std::any Accept(SequentVisitor& visitor) override;

    void SetParent(SequentNode* p);

    SequentNode* parent = nullptr;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_UNARYSEQUENTNODE_HPP

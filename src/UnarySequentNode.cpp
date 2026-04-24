//
// Created by Andrew Nazareth on 4/14/26.
//

#include "UnarySequentNode.hpp"

namespace Logic_Project {
UnarySequentNode::UnarySequentNode(const std::vector<std::unique_ptr<LogicExpression>>& antecedents, const std::vector<std::unique_ptr<LogicExpression>>& succedents)
{
    for (const auto& expr : antecedents) {
        this->antecedents.push_back(expr->Copy());
    }
    for (const auto& expr : succedents) {
        this->succedents.push_back(expr->Copy());
    }
    this->rule = SequentNodeRule::None;
}
UnarySequentNode::UnarySequentNode(const std::vector<std::unique_ptr<LogicExpression>>& antecedents, const std::vector<std::unique_ptr<LogicExpression>>& succedents, const SequentNodeRule& rule)
{
    for (const auto& expr : antecedents) {
        this->antecedents.push_back(expr->Copy());
    }
    for (const auto& expr : succedents) {
        this->succedents.push_back(expr->Copy());
    }
    this->rule = rule;
}
void UnarySequentNode::SetParent(SequentNode* p)
{
    this->parent = p;
    if (this->parent != nullptr) {
        this->parent->child = this;
    }
}
std::unique_ptr<TreeNode> UnarySequentNode::Accept(SequentVisitor& visitor)
{
    return visitor.Visit(*this);
}
}
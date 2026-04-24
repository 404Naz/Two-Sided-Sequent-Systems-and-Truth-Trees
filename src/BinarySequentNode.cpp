//
// Created by Andrew Nazareth on 4/14/26.
//
#include "BinarySequentNode.hpp"
#include "SequentNode.hpp"

#include <utility>

namespace Logic_Project {
BinarySequentNode::BinarySequentNode(const std::vector<std::unique_ptr<LogicExpression>>& antecedents, const std::vector<std::unique_ptr<LogicExpression>>& succedents)
{
    for (const auto& expr : antecedents) {
        this->antecedents.push_back(expr->Copy());
    }
    for (const auto& expr : succedents) {
        this->succedents.push_back(expr->Copy());
    }
    this->rule = SequentNodeRule::None;
}
BinarySequentNode::BinarySequentNode(const std::vector<std::unique_ptr<LogicExpression>>& antecedents, const std::vector<std::unique_ptr<LogicExpression>>& succedents, const SequentNodeRule& rule)
{
    for (const auto& expr : antecedents) {
        this->antecedents.push_back(expr->Copy());
    }
    for (const auto& expr : succedents) {
        this->succedents.push_back(expr->Copy());
    }
    this->rule = rule;
}
void BinarySequentNode::SetLeftParent(SequentNode* left)
{
    this->leftParent = left;
    if (this->leftParent != nullptr) {
        this->leftParent->child = this;
    }
}
void BinarySequentNode::SetRightParent(SequentNode* right)
{
    this->rightParent = right;
    if (this->rightParent != nullptr) {
        this->rightParent->child = this;
    }
}
std::unique_ptr<TreeNode> BinarySequentNode::Accept(SequentVisitor& visitor)
{
    return visitor.Visit(*this);
}

}
//
// Created by Andrew Nazareth on 3/23/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_LOGICALAND_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_LOGICALAND_HPP

#include "BinaryLogicExpression.hpp"
#include "LogicExpression.hpp"

namespace Logic_Project {

template <typename LeftT, typename RightT>
class LogicalAnd;

// General Form
template <>
class LogicalAnd<
    LogicExpression, LogicExpression> : public BinaryLogicExpression<LogicalAnd> {
public:
    using BinaryLogicExpression::BinaryLogicExpression;

    [[nodiscard]] std::string Serialize() const override
    {
        return std::format("({} ∧ {})", this->GetLeftOperand().Serialize(), this->GetRightOperand().Serialize());
    }

    EXPRESSION_TYPE(And);
    EXPRESSION_CATEGORY(BinaryExpression);
};

template <typename LeftT = LogicExpression, typename RightT = LeftT>
class LogicalAnd final : public BinaryLogicExpression<LogicalAnd, LeftT, RightT>
{
public:
    LogicalAnd() = default;
    LogicalAnd(const LogicalAnd& other) :
    BinaryLogicExpression<LogicalAnd, LeftT, RightT>(other) {}

    LogicalAnd(const LeftT& left, const RightT& right) :
    BinaryLogicExpression<LogicalAnd, LeftT, RightT>(left, right) {}

    LogicalAnd& operator=(const LogicalAnd& other) = default;
    [[nodiscard]] std::string Serialize() const override
    {
        return std::format("({} ∧ {})", this->GetLeftOperand().Serialize(), this->GetRightOperand().Serialize());
    }

    EXPRESSION_TYPE(And);
    EXPRESSION_CATEGORY(BinaryExpression);
};


}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_LOGICALAND_HPP

//
// Created by Andrew Nazareth on 3/27/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_LOGICALOR_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_LOGICALOR_HPP

#include "BinaryLogicExpression.hpp"
#include "LogicExpression.hpp"

namespace Logic_Project {

template <typename LeftT, typename RightT>
class LogicalOr;

// General Form
template <>
class LogicalOr<
    LogicExpression, LogicExpression> : public BinaryLogicExpression<LogicalOr> {
public:
    using BinaryLogicExpression::BinaryLogicExpression;

    EXPRESSION_TYPE(Or);
    EXPRESSION_CATEGORY(BinaryExpression);
};

template <typename LeftT = LogicExpression, typename RightT = LeftT>
class LogicalOr final : public BinaryLogicExpression<LogicalOr, LeftT, RightT>
{
public:
    LogicalOr() = default;
    LogicalOr(const LogicalOr& other) :
    BinaryLogicExpression<LogicalOr, LeftT, RightT>(other) {}

    LogicalOr(const LeftT& left, const RightT& right) :
    BinaryLogicExpression<LogicalOr, LeftT, RightT>(left, right) {}

    LogicalOr& operator=(const LogicalOr& other) = default;
    [[nodiscard]] std::string Serialize() const override
    {
        return std::format("({} ∨ {})", this->GetLeftOperand().Serialize(), this->GetRightOperand().Serialize());
    }

    EXPRESSION_TYPE(Or);
    EXPRESSION_CATEGORY(BinaryExpression);
};


}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_LOGICALOR_HPP

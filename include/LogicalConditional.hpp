//
// Created by Andrew Nazareth on 3/27/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_LOGICALCONDITIONAL_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_LOGICALCONDITIONAL_HPP

#include "BinaryLogicExpression.hpp"
#include "LogicExpression.hpp"

namespace Logic_Project {

template <typename LeftT, typename RightT>
class LogicalConditional;

// General Form
template <>
class LogicalConditional<
    LogicExpression, LogicExpression> : public BinaryLogicExpression<LogicalConditional> {
public:
    using BinaryLogicExpression::BinaryLogicExpression;

    EXPRESSION_TYPE(Conditional);
    EXPRESSION_CATEGORY(BinaryExpression);
};

template <typename LeftT = LogicExpression, typename RightT = LeftT>
class LogicalConditional final : public BinaryLogicExpression<LogicalConditional, LeftT, RightT>
{
public:
    LogicalConditional() = default;
    LogicalConditional(const LogicalConditional& other) :
    BinaryLogicExpression<LogicalConditional, LeftT, RightT>(other) {}

    LogicalConditional& operator=(const LogicalConditional& other) = default;

    EXPRESSION_TYPE(Conditional);
    EXPRESSION_CATEGORY(BinaryExpression);
};


}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_LOGICALCONDITIONAL_HPP

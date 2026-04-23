//
// Created by Andrew Nazareth on 3/24/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_LOGICALNOT_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_LOGICALNOT_HPP
#include "LogicExpression.hpp"
#include "UnaryLogicExpression.hpp"

namespace Logic_Project {

template <typename OperandT>
class LogicalNot;

template <>
class LogicalNot<LogicExpression> : public UnaryLogicExpression<LogicalNot> {
public:
    using UnaryLogicExpression::UnaryLogicExpression;

    [[nodiscard]] std::string Serialize() const override;

    EXPRESSION_TYPE(Not);
    EXPRESSION_CATEGORY(UnaryExpression);
};

template <typename OperandT = LogicExpression>
class LogicalNot final : public UnaryLogicExpression<LogicalNot, OperandT> {
    public:
    LogicalNot() = default;
    LogicalNot(const LogicalNot& other) : UnaryLogicExpression<LogicalNot, OperandT>(other) {}
    explicit LogicalNot(const OperandT& op) : UnaryLogicExpression<LogicalNot, OperandT>(op) {}

    LogicalNot& operator=(const LogicalNot& other) = default;

    [[nodiscard]] std::string Serialize() const override
    {
        return std::format("¬{}", this->GetOperand().Serialize());
    }

    EXPRESSION_TYPE(Not)
    EXPRESSION_CATEGORY(UnaryExpression)
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_LOGICALNOT_HPP

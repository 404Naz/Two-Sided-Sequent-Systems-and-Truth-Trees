//
// Created by Andrew Nazareth on 3/21/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_UNARYLOGICEXPRESSION_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_UNARYLOGICEXPRESSION_HPP
#include "LogicExpression.hpp"
#include "RecursiveCast.hpp"
#include <cassert>

namespace Logic_Project {
/**
 * A unary expression, such as NOT
 * @tparam DerivedT Type of the derived class
 * @tparam OpT Type of the operand
 */
template <template<IExpression> class DerivedT, IExpression OpT = LogicExpression>
class UnaryLogicExpression : public LogicExpression {
public:
    using DerivedSpecialized = DerivedT<OpT>;
    using DerivedGeneralized = DerivedT<LogicExpression>;
    UnaryLogicExpression() = default;
    UnaryLogicExpression(const UnaryLogicExpression& other)
        : LogicExpression(other)
    {
        if (other.HasOperand()) {
            SetOperand(other.GetOperand());
        }
    }
    explicit UnaryLogicExpression(const OpT& op)
    {
        SetOperand(op);
    }
    [[nodiscard]] std::unique_ptr<LogicExpression> Copy() const final
    {
        return std::make_unique<DerivedSpecialized>(*static_cast<const DerivedSpecialized*>(this));
    }
    [[nodiscard]] bool Equals(const LogicExpression& other) const final
    {
        if ((this->GetCategory() != other.GetCategory()) || (this->GetType() != other.GetType())) {
            return false;
        }

        // Other should be a unary expression so it should have one operand
        const auto otherGeneralized = other.Generalize();
        const auto& otherUnaryGeneralized = static_cast<const DerivedGeneralized&>(*otherGeneralized);

        return this->operand->Equals(otherUnaryGeneralized.GetOperand());
    }
    [[nodiscard]] bool HasOperand() const
    {
        return this->operand != nullptr;
    }
    [[nodiscard]] const OpT& GetOperand() const
    {
        assert (operand != nullptr);
        return *operand;
    }
    [[nodiscard]] auto Generalize() const -> std::unique_ptr<LogicExpression> final
    {
        DerivedGeneralized generalized;
        if (this->operand) {
            generalized.SetOperand(*this->operand->Copy());
        }
        return std::make_unique<DerivedGeneralized>(generalized);
    }

    template <typename T> requires IsAnyOf<T, OpT, LogicExpression>
    bool SetOperand(const T& op)
    {
        if constexpr (std::same_as<OpT, LogicExpression>) {
            this->operand = op.Copy();
            return true;
        }

        if constexpr (std::same_as<OpT, T> && !std::same_as<OpT, LogicExpression>) {
            this->operand = std::make_unique<OpT>(op);
            return true;
        }

        if (auto castedOp = RecursiveCast<OpT>(op); castedOp) {
            operand = std::move(castedOp);
            return true;
        }
        return false;
    }

    [[nodiscard]] std::string Serialize() const override
    {
        return Generalize()->Serialize();
    }

    EXPRESSION_CATEGORY(UnaryExpression)
private:
    std::unique_ptr<OpT> operand;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_UNARYLOGICEXPRESSION_HPP

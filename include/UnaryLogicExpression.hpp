//
// Created by Andrew Nazareth on 3/21/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_UNARYLOGICEXPRESSION_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_UNARYLOGICEXPRESSION_HPP
#include "LogicExpression.hpp"
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
    {
        this->operand = other.operand->Copy();
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
    [[nodiscard]] const OpT& GetOperand() const
    {
        assert (operand != nullptr);
        return *operand;
    }

    EXPRESSION_CATEGORY(UnaryExpression)
private:
    void SetOperand(const OpT& op)
    {
        assert(op != nullptr);
        this->operand = op;
    }
    std::unique_ptr<OpT> operand;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_UNARYLOGICEXPRESSION_HPP

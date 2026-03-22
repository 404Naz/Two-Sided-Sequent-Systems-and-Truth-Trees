//
// Created by Andrew Nazareth on 3/21/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_UNARYLOGICEXPRESSION_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_UNARYLOGICEXPRESSION_HPP
#include "LogicExpression.hpp"

namespace Logic_Project {
/**
 * A unary expression, such as NOT
 * @tparam DerivedT Type of the derived class
 * @tparam OpT Type of the operand
 */
template <template<IExpression> class DerivedT, typename OpT>
class UnaryLogicExpression : public LogicExpression {
public:
    using DerivedSpecialized = DerivedT<OpT>;
    using DerivedGeneralized = DerivedT<LogicExpression>;
    UnaryLogicExpression() = delete;
    UnaryLogicExpression(const UnaryLogicExpression& other)
    {
        SetOperand(other.operand);
    }
    explicit UnaryLogicExpression(const OpT& op)
    {
        SetOperand(op);
    }
    [[nodiscard]] std::unique_ptr<LogicExpression> Copy() const final
    {
        return std::unique_ptr<LogicExpression>(*static_cast<const DerivedSpecialized*>(this));
    }
    [[nodiscard]] bool Equals(const LogicExpression& other) const final
    {
        if ((this->GetCategory() != other.GetCategory()) || (this->GetType() != other.GetType())) {
            return false;
        }

        // Other should be a binary expression so it should have a left and right operand.
        const auto otherGeneralized = other.Generalize();
        const auto& otherUnaryGeneralized = static_cast<const DerivedGeneralized&>(*otherGeneralized);

        return this->operand->Equals(otherUnaryGeneralized.GetOperand());
    }
    [[nodiscard]] std::unique_ptr<LogicExpression> GetOperand() const
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

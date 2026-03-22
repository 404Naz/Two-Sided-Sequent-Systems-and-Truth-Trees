//
// Created by Andrew Nazareth on 3/20/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_BINARYLOGICEXPRESSION_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_BINARYLOGICEXPRESSION_HPP
#include "LogicExpression.hpp"
#include <cassert>

namespace Logic_Project {

/**
 * A binary logic expression, such as logical AND or logical OR
 * @tparam DerivedT Type of the derived class
 * @tparam LeftT Type of the left operand
 * @tparam RightT Type of the right operand
 */
template <template<IExpression, IExpression> class DerivedT, typename LeftT, typename RightT>
class BinaryLogicExpression : public LogicExpression {
    using DerivedSpecialized = DerivedT<LeftT, RightT>;
    using DerivedGeneralized = DerivedT<LogicExpression, LogicExpression>;
public:
    BinaryLogicExpression() = delete;
    BinaryLogicExpression(const BinaryLogicExpression& other)
    {
        SetLeftOp(other.GetLeftOperand());
        SetRightOp(other.GetRightOperand());
    }

    BinaryLogicExpression(const LeftT& leftOp, const RightT& rightOp)
    {
        SetLeftOp(leftOp);
        SetRightOp(rightOp);
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
        const auto& otherBinaryGeneralized = static_cast<const DerivedGeneralized&>(*otherGeneralized);

        return this->left->Equals(otherBinaryGeneralized.GetLeftOperand()) && \
               this->right->Equals(otherBinaryGeneralized.GetRightOperand());
    }

    /**
     * Gets the left operand.
     * @return The left operand
     */
    [[nodiscard]] std::unique_ptr<LogicExpression> GetLeftOperand() const
    {
        assert(left != nullptr);
        return *left;
    }
    /**
     * Gets the right operand.
     * @return The right operand
     */
    [[nodiscard]] std::unique_ptr<LogicExpression> GetRightOperand() const
    {
        assert(right != nullptr);
        return *right;
    }

    EXPRESSION_CATEGORY(BinaryExpression)
private:
    /**
     * Sets the least significant operand of this expression.
     * @param op the operand to set
     */
    template <typename T> requires IsAnyOf<T, LeftT, LogicExpression>
    void SetLeftOp(const LeftT& op)
    {
        this->left = op;
    }
    /**
     * Sets the least significant operand of this expression.
     * @param op the operand to set
     */
    template <typename T> requires IsAnyOf<T, RightT, LogicExpression>
    void SetRightOp(const RightT& op)
    {
        this->right = op;
    }
    std::unique_ptr<LeftT> left;
    std::unique_ptr<RightT> right;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_BINARYLOGICEXPRESSION_HPP

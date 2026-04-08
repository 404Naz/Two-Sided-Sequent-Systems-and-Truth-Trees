//
// Created by Andrew Nazareth on 3/20/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_BINARYLOGICEXPRESSION_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_BINARYLOGICEXPRESSION_HPP
#include "LogicExpression.hpp"
#include "RecursiveCast.hpp"
#include <cassert>

namespace Logic_Project {

/**
 * A binary logic expression, such as logical AND or logical OR
 * @tparam DerivedT Type of the derived class
 * @tparam LeftT Type of the left operand
 * @tparam RightT Type of the right operand
 */
template <template<IExpression, IExpression> class DerivedT, IExpression LeftT = LogicExpression, IExpression RightT = LeftT>
class BinaryLogicExpression : public LogicExpression {
    using DerivedSpecialized = DerivedT<LeftT, RightT>;
    using DerivedGeneralized = DerivedT<LogicExpression, LogicExpression>;
public:
    BinaryLogicExpression() = default;
    BinaryLogicExpression(const BinaryLogicExpression& other)
    {
        if (other.HasLeftOperand() and other.HasRightOperand()) {
            SetLeftOperand(other.GetLeftOperand());
            SetRightOperand(other.GetRightOperand());
        }
    }

    explicit BinaryLogicExpression(const LeftT& leftOp, const RightT& rightOp)
    {
        SetLeftOperand(leftOp);
        SetRightOperand(rightOp);
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

        // Other should be a binary expression so it should have a left and right operand.
        const auto otherGeneralized = other.Generalize();
        const auto& otherBinaryGeneralized = static_cast<const DerivedGeneralized&>(*otherGeneralized);

        return this->left->Equals(otherBinaryGeneralized.GetLeftOperand()) && \
               this->right->Equals(otherBinaryGeneralized.GetRightOperand());
    }

    [[nodiscard]] std::string Serialize() const override
    {
        return Generalize()->Serialize();
    }

    [[nodiscard]] bool HasLeftOperand() const
    {
        return this->left != nullptr;
    }

    [[nodiscard]] bool HasRightOperand() const
    {
        return this->right != nullptr;
    }

    /**
     * Gets the left operand.
     * @return The left operand
     */
    [[nodiscard]] const LeftT& GetLeftOperand() const
    {
        assert(left != nullptr);
        return *left;
    }
    /**
     * Gets the right operand.
     * @return The right operand
     */
    [[nodiscard]] const RightT& GetRightOperand() const
    {
        assert(right != nullptr);
        return *right;
    }

    EXPRESSION_CATEGORY(BinaryExpression)
private:
    /**
     * Sets the left operand of this expression.
     * @param op the operand to set
     */
    template <typename T> requires IsAnyOf<T, LeftT, LogicExpression>
    bool SetLeftOperand(const T& op)
    {
        if constexpr (std::same_as<LeftT, LogicExpression>) {
            this->left = op.Copy();
            return true;
        }

        if constexpr (std::same_as<LeftT, T> && !std::same_as<LeftT, LogicExpression>) {
            this->left = std::make_unique<LeftT>(op);
            return true;
        }

        if (auto castedOp = RecursiveCast<LeftT>(op); castedOp) {
            left = std::move(castedOp);
            return true;
        }
        return false;
    }
    /**
     * Sets the right operand of this expression.
     * @param op the operand to set
     */
    template <typename T> requires IsAnyOf<T, RightT, LogicExpression>
    bool SetRightOperand(const T& op)
    {
        if constexpr (std::same_as<RightT, LogicExpression>) {
            this->right = op.Copy();
            return true;
        }

        if constexpr (std::same_as<RightT, T> && !std::same_as<RightT, LogicExpression>) {
            this->right = std::make_unique<RightT>(op);
            return true;
        }

        if (auto castedOp = RecursiveCast<RightT>(op); castedOp) {
            right = std::move(castedOp);
            return true;
        }

        return false;
    }
    std::unique_ptr<LeftT> left;
    std::unique_ptr<RightT> right;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_BINARYLOGICEXPRESSION_HPP

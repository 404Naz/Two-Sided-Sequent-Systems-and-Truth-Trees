//
// Created by Andrew Nazareth on 3/20/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_LOGICEXPRESSION_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_LOGICEXPRESSION_HPP
#include <memory>
#include "Concepts.hpp"

namespace Logic_Project {
enum class ExpressionType {
    And,
    Or,
    Not,
    Conditional,
    Biconditional,
    Atomic
};

enum class ExpressionCategory {
    LeafExpression,
    UnaryExpression,
    BinaryExpression,
};

class LogicExpression {
public:
    virtual ~LogicExpression() = default;
    LogicExpression();
    /**
     * Copies this expression.
     * @return A copy of this expression.
     */
    [[nodiscard]] virtual std::unique_ptr<LogicExpression> Copy() const = 0;

    /**
     * Compares this expression to another expression for equality.
     * Two expressions are equal if they are structurally equivalent and have the same value.
     *
     * Two expressions are equal if they are structurally equivalent and have the same value.
     * @param other The other expression.
     * @return true if equal, false otherwise
     */
    [[nodiscard]] virtual bool Equals(const LogicExpression& other) const = 0;

    /**
     * Gets the category of this expression.
     * @return The category of this expression.
     */
    [[nodiscard]] virtual auto GetCategory() const -> ExpressionCategory;

    /**
     * Gets the type of this expression.
     * @return The type of this expression.
     */
    [[nodiscard]] virtual auto GetType() const -> ExpressionType;

    /**
     * Converts this expression to a more general expression.
     *
     * Some expressions may explicitly specify the type of their operands. For example, a
     * `And<Atom>` expression may only accept `Atom` operands. This function converts the
     * expression to a more general expression, such as `And<LogicExpression>`, which
     * accepts any expression as an operand.
     *
     * @return The generalized expression.
     */
    [[nodiscard]] virtual auto Generalize() const -> std::unique_ptr<LogicExpression> = 0;

    /**
     * Gets whether this expression is of a specific type.
     *
     * @tparam T The type to check against.
     * @return true if this expression is of type T, false otherwise.
     */
    template <class T>
    [[nodiscard]] bool Is() const
    {
        return GetType() == T::GetStaticType();
    }

    #define EXPRESSION_TYPE(type)                   \
    auto GetType() const -> ExpressionType override \
    {                                               \
        return ExpressionType::type;                \
    }                                               \
                                                    \
    static auto GetStaticType() -> ExpressionType   \
    {                                               \
        return ExpressionType::type;                \
    }
    #define EXPRESSION_CATEGORY(category)                   \
    auto GetCategory() const -> ExpressionCategory override \
    {                                                       \
        return ExpressionCategory::category;                                    \
    }                                                       \
                                                            \
    constexpr static auto GetStaticCategory() -> ExpressionCategory \
    {                                                               \
        return ExpressionCategory::category;                                            \
    }
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_LOGICEXPRESSION_HPP

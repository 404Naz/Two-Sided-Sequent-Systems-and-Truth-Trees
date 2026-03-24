//
// Created by Andrew Nazareth on 3/24/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_RECURSIVECAST_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_RECURSIVECAST_HPP

#include "LogicExpression.hpp"

namespace Logic_Project {

/**
 * Recursively casts type expression to type T
 * @tparam T Type to cast to
 * @param expr Expression to cast
 * @return unique pointer of expression casted to type T or nullptr
 */
template <IExpression T> requires DerivedFromBinaryExpression<T>
std::unique_ptr<T> RecursiveCast(LogicExpression& expr)
{
    if constexpr (std::is_same_v<T, LogicExpression>) {
        return expr.Copy();
    } else {
        if (expr.Is<T>())
        {
            return std::make_unique<T>(expr);
        } else {
            return nullptr;
        }
    }
}

/**
 * Recursively casts type expression to type T
 * @tparam T Type to cast to
 * @param expr Expression to cast
 * @return unique pointer of expression casted to type T or nullptr
 */
template <IExpression T> requires DerivedFromUnaryExpression<T>
std::unique_ptr<T> RecursiveCast(LogicExpression& expr)
{
    if constexpr (std::is_same_v<T, LogicExpression>) {
        return expr.Copy();
    } else {
        if (expr.Is<T>())
        {
            return std::make_unique<T>(expr);
        } else {
            return nullptr;
        }
    }
}

/**
 * Recursively casts type expression to type T
 * @tparam T Type to cast to
 * @param expr Expression to cast
 * @return unique pointer of expression casted to type T or nullptr
 */
template <IExpression T> requires (!DerivedFromBinaryExpression<T> && !DerivedFromUnaryExpression<T>)
std::unique_ptr<T> RecursiveCast(LogicExpression& expr)
{
    if constexpr (std::is_same_v<T, LogicExpression>) {
        return expr.Copy();
    } else {
        return expr.Is<T>() ? std::make_unique<T>(dynamic_cast<const T&>(expr)) : nullptr;
    }
}

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_RECURSIVECAST_HPP

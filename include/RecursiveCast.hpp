//
// Created by Andrew Nazareth on 3/24/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_RECURSIVECAST_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_RECURSIVECAST_HPP

#include "LogicExpression.hpp"
#include "BinaryLogicExpression.hpp"
#include "UnaryLogicExpression.hpp"

// Code taken from OASIS

namespace Logic_Project {

/**
 * Recursively casts type expression to type T
 * @tparam T Type to cast to
 * @param other Expression to cast
 * @return unique pointer of expression casted to type T or nullptr
 */
template <IExpression T>
    requires DerivedFromBinaryExpression<T>
auto RecursiveCast(const LogicExpression& other) -> std::unique_ptr<T>
{
    T dummy;
    return [&other]<template <typename, typename> typename DerivedT, typename MostSigOpT, typename LeastSigOpT>(BinaryLogicExpression<DerivedT, MostSigOpT, LeastSigOpT>&) -> std::unique_ptr<T> {
        if (!other.Is<DerivedT<LogicExpression, LogicExpression>>()) {
            return nullptr;
        }

        const std::unique_ptr<LogicExpression> otherGeneralized = other.Generalize();
        const auto& otherBinaryExpression = static_cast<const DerivedT<LogicExpression, LogicExpression>&>(*otherGeneralized);

        auto specializedMostSigOp = RecursiveCast<MostSigOpT>(otherBinaryExpression.GetMostSigOp());
        auto specializedLeastSigOp = RecursiveCast<LeastSigOpT>(otherBinaryExpression.GetLeastSigOp());

        if (specializedMostSigOp && specializedLeastSigOp) {
            return std::make_unique<T>(*specializedMostSigOp, *specializedLeastSigOp);
        }

        auto otherWithSwappedOps
            = otherBinaryExpression.SwapOperands();

        specializedMostSigOp = RecursiveCast<MostSigOpT>(otherWithSwappedOps.GetMostSigOp());
        specializedLeastSigOp = RecursiveCast<LeastSigOpT>(otherWithSwappedOps.GetLeastSigOp());

        if (specializedMostSigOp && specializedLeastSigOp) {
            return std::make_unique<T>(*specializedMostSigOp, *specializedLeastSigOp);
        }

        return nullptr;
    }(dummy);
}

template <IExpression T>
    requires DerivedFromUnaryExpression<T>
auto RecursiveCast(const LogicExpression& other) -> std::unique_ptr<T>
{
    T dummy;
    return [&other]<template <typename> typename DerivedT, typename OpT>(UnaryLogicExpression<DerivedT, OpT>&) -> std::unique_ptr<T> {
        if (!other.Is<DerivedT<LogicExpression>>()) {
            return nullptr;
        }

        const std::unique_ptr<LogicExpression> otherGeneralized = other.Generalize();
        const auto& otherUnaryExpression = static_cast<const DerivedT<LogicExpression>&>(*otherGeneralized);

        auto specializedOp = RecursiveCast<OpT>(otherUnaryExpression.GetOperand());
        if (specializedOp) {
            return std::make_unique<T>(*specializedOp);
        }

        return nullptr;
    }(dummy);
}

template <IExpression T>
    requires(!DerivedFromBinaryExpression<T> && !DerivedFromUnaryExpression<T>)
auto RecursiveCast(const LogicExpression& other) -> std::unique_ptr<T>
{
    if constexpr (std::is_same_v<T, LogicExpression>)
        return other.Copy();
    else
        return other.Is<T>() ? std::make_unique<T>(dynamic_cast<const T&>(other)) : nullptr;
}

/**
 * Recursively casts type expression to type T
 * @tparam T Type to cast to
 * @param expr Expression to cast
 * @return unique pointer of expression casted to type T or nullptr
 */
// template <IExpression T> requires DerivedFromBinaryExpression<T>
// std::unique_ptr<T> RecursiveCast(const LogicExpression& expr)
// {
//     if constexpr (std::is_same_v<T, LogicExpression>) {
//         return expr.Copy();
//     } else {
//         if (expr.Is<T>())
//         {
//             return std::make_unique<T>(expr);
//         } else {
//             return nullptr;
//         }
//     }
// }

/**
 * Recursively casts type expression to type T
 * @tparam T Type to cast to
 * @param expr Expression to cast
 * @return unique pointer of expression casted to type T or nullptr
 */
// template <IExpression T> requires DerivedFromUnaryExpression<T>
// std::unique_ptr<T> RecursiveCast(const LogicExpression& expr)
// {
//     if constexpr (std::is_same_v<T, LogicExpression>) {
//         return expr.Copy();
//     } else {
//         if (expr.Is<T>())
//         {
//             return std::make_unique<T>(expr);
//         } else {
//             return nullptr;
//         }
//     }
// }

/**
 * Recursively casts type expression to type T
 * @tparam T Type to cast to
 * @param expr Expression to cast
 * @return unique pointer of expression casted to type T or nullptr
 */
// template <IExpression T> requires (!DerivedFromBinaryExpression<T> && !DerivedFromUnaryExpression<T>)
// std::unique_ptr<T> RecursiveCast(const LogicExpression& expr)
// {
//     if constexpr (std::is_same_v<T, LogicExpression>) {
//         return expr.Copy();
//     } else {
//         return expr.Is<T>() ? std::make_unique<T>(dynamic_cast<const T&>(expr)) : nullptr;
//     }
// }

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_RECURSIVECAST_HPP

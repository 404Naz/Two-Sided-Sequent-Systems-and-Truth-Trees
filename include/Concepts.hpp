//
// Created by Andrew Nazareth on 3/20/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_CONCEPTS_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_CONCEPTS_HPP
#include <concepts>

namespace Logic_Project {

// Forward declarations
class LogicExpression;
enum class ExpressionType;
enum class ExpressionCategory;

/**
 * Checks if type T is same as any of the provided types in U.
 *
 * @tparam T The type to compare against.
 * @tparam U The comparision types.
 * @return true if T is same as any type in U, false otherwise.
 */
template <typename T, typename... U>
concept IsAnyOf = (std::same_as<T, U> || ...);

/**
 * An expression concept.
 * @note This class is not intended to be used directly by end users.
 *
 * An expression concept is a type that satisfies the following requirements:
 * - It is derived from `LogicExpression`.
 * - It has a static `Specialize` function that returns a `std::unique_ptr<T>` and takes a `const Expression&` as an argument.
 * - It has a static `Specialize` function that returns a `std::unique_ptr<T>` and takes a `const Expression&` and a `tf::Subflow&` as arguments.
 * - It has a static `GetStaticCategory` function that returns a `uint32_t`.
 * - It has a static `GetStaticType` function that returns an `ExpressionType`.
 *
 * @tparam T The type to check.
 */
template <typename T>
concept IExpression = (requires(T, const LogicExpression& other) {
    {
        T::GetStaticCategory()
    } -> std::same_as<ExpressionCategory>;
    {
        T::GetStaticType()
    } -> std::same_as<ExpressionType>;
} && std::derived_from<T, LogicExpression>) || std::is_same_v<T, LogicExpression>;


}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_CONCEPTS_HPP

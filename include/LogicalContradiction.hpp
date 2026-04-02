//
// Created by Andrew Nazareth on 3/22/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_LOGICALATOM_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_LOGICALATOM_HPP
#include "LeafLogicExpression.hpp"
#include "LogicExpression.hpp"
#include <string>

namespace Logic_Project {

/**
 * An atom (zero place predicate)
 */
class LogicalContradiction : public LeafLogicExpression<LogicalContradiction> {
    public:
    LogicalContradiction() = delete;
    LogicalContradiction(const LogicalContradiction &) = default;

    [[nodiscard]] bool Equals(const LogicExpression& other) const final
    {
        return other.Is<LogicalContradiction>();
    }

    EXPRESSION_TYPE(Contradiction)
    EXPRESSION_CATEGORY(LeafExpression)
};

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_LOGICALATOM_HPP

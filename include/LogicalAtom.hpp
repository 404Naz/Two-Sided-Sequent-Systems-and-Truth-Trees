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
class LogicalAtom : public LeafLogicExpression<LogicalAtom> {
    public:
    LogicalAtom() = delete;
    LogicalAtom(const LogicalAtom &) = default;
    explicit LogicalAtom(const std::string& rep)
    {
        representation = rep;
    }

    [[nodiscard]] bool Equals(const LogicExpression& other) const final;

    std::string GetRepresentation();

    EXPRESSION_TYPE(Atomic)
    EXPRESSION_CATEGORY(LeafExpression)
    private:
    std::string representation;
};

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_LOGICALATOM_HPP

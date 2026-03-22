//
// Created by Andrew Nazareth on 3/22/26.
//

#include "LogicalAtom.hpp"

namespace Logic_Project {
std::string LogicalAtom::GetRepresentation()
{
    return representation;
}

bool LogicalAtom::Equals(const LogicExpression& other) const
{
    return other.Is<LogicalAtom>() && representation == dynamic_cast<const LogicalAtom&>(other).representation;
}

}
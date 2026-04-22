//
// Created by Andrew Nazareth on 3/24/26.
//

#include "LogicalNot.hpp"

namespace Logic_Project {

[[nodiscard]] std::string LogicalNot<>::Serialize() const
{
    return std::format("¬{}", this->GetOperand().Serialize());
}

}
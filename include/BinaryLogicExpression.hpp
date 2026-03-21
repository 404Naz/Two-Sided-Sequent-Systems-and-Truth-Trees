//
// Created by Andrew Nazareth on 3/20/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_BINARYLOGICEXPRESSION_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_BINARYLOGICEXPRESSION_HPP
#include "LogicExpression.hpp"

namespace Logic_Project {
class BinaryLogicExpression : public LogicExpression {
public:
    BinaryLogicExpression();

    virtual bool Equals();
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_BINARYLOGICEXPRESSION_HPP

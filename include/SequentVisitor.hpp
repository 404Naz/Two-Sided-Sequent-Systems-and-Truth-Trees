//
// Created by Andrew Nazareth on 3/31/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTVISITOR_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTVISITOR_HPP

#include "LogicExpression.hpp"
#include <any>

namespace Logic_Project {

class UnarySequentNode;
class BinarySequentNode;

class SequentVisitor {
public:
    SequentVisitor() = default;
    virtual ~SequentVisitor() = default;
    virtual std::any Visit(const UnarySequentNode& unary) = 0;
    virtual std::any Visit(const BinarySequentNode& binary) = 0;
};

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTVISITOR_HPP

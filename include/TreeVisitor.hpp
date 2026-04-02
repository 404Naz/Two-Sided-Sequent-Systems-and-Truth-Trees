//
// Created by Andrew Nazareth on 3/31/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_TREEVISITOR_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_TREEVISITOR_HPP

#include "LogicExpression.hpp"

#include <any>

namespace Logic_Project {

class UnaryTreeNode;
class BinaryTreeNode;
class ClosedTreeNode;
class OpenTreeNode;

class TreeVisitor {
public:
    TreeVisitor();
    virtual ~TreeVisitor() = 0;
    virtual std::any Visit(const UnaryTreeNode& unary) = 0;
    virtual std::any Visit(const BinaryTreeNode& binary) = 0;
    virtual std::any Visit(const ClosedTreeNode& closed) = 0;
    virtual std::any Visit(const OpenTreeNode& open) = 0;
};

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_TREEVISITOR_HPP

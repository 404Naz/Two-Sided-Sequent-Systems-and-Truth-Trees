//
// Created by Andrew Nazareth on 3/31/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTNODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTNODE_HPP
#include "LogicExpression.hpp"

#include <SequentVisitor.hpp>
#include <any>
#include <vector>

namespace Logic_Project {

enum class SequentNodeRule {
    None,
    NegL,
    NegR,
    ConjL,
    ConjR,
    DisjL,
    DisjR,
    ImplL,
    ImplR,
    WL,
    WR,
    CL,
    CR,
    XL,
    XR,
};

class SequentNode {
    public:
    SequentNode() = default;
    virtual ~SequentNode() = default;

    virtual std::any Accept(SequentVisitor& visitor) = 0;

    SequentNodeRule rule = SequentNodeRule::None;
    std::vector<std::unique_ptr<LogicExpression>> antecedents;
    std::vector<std::unique_ptr<LogicExpression>> succedents;
    SequentNode* child = nullptr;
    bool isRoot = false;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTNODE_HPP

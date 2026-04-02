//
// Created by Andrew Nazareth on 3/31/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTNODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTNODE_HPP
#include "LogicExpression.hpp"

#include <vector>

namespace Logic_Project {

enum class SequentNodeRule {
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

    SequentNodeRule rule;
    std::vector<std::unique_ptr<LogicExpression>> antecedents;
    std::vector<std::unique_ptr<LogicExpression>> succedents;
    std::unique_ptr<SequentNode> child;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_SEQUENTNODE_HPP

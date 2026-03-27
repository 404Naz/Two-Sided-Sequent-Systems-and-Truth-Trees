//
// Created by Andrew Nazareth on 3/27/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_TREENODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_TREENODE_HPP
#include <vector>
#include "LogicExpression.hpp"

#include <queue>

namespace Logic_Project {
class TreeNode {
    public:
    TreeNode() = default;

    // Statement represented by this node
    std::unique_ptr<LogicExpression> statement;

    // Expressions that haven't been decomposed
    std::queue<std::unique_ptr<LogicExpression>> expressions;

    // Atomics or their negations
    std::vector<std::unique_ptr<LogicExpression>> env;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_TREENODE_HPP

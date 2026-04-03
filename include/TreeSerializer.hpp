//
// Created by Andrew Nazareth on 3/31/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_TREESERIALIZER_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_TREESERIALIZER_HPP

#include "TreeVisitor.hpp"

#include <TreeNode.hpp>
#include <any>
#include <deque>
#include <vector>

namespace Logic_Project {

class TreeSerializer final : public TreeVisitor {
    public:
    TreeSerializer() = default;

    std::string Serialize();
    [[nodiscard]] std::string Serialize(const TreeNode& root);

    std::any Visit(const UnaryTreeNode& node) override;
    std::any Visit(const BinaryTreeNode& node) override;
    std::any Visit(const ClosedTreeNode& node) override;
    std::any Visit(const OpenTreeNode& node) override;

    // TODO: Do I need this here? Could this be in a different visitor?
    // Expressions that haven't been decomposed
    std::deque<std::unique_ptr<LogicExpression>> expressions;
    // Atomics or their negations
    std::vector<std::unique_ptr<LogicExpression>> env;

    std::vector<std::string> serializedNodes;
};

}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_TREESERIALIZER_HPP

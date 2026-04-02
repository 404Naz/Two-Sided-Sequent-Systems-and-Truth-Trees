//
// Created by Andrew Nazareth on 3/27/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_TREENODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_TREENODE_HPP
#include "LogicExpression.hpp"
#include "TreeVisitor.hpp"
#include <vector>

#include <any>
#include <deque>

namespace Logic_Project {
class TreeNode {
    public:
    TreeNode();
    TreeNode(const TreeNode& node);
    virtual ~TreeNode() = default;

    virtual std::any Accept(TreeVisitor& visitor) = 0;

    void SetParent(const TreeNode& node);
    [[nodiscard]] bool HasParent() const;

    [[nodiscard]] virtual std::unique_ptr<TreeNode> Copy() const = 0;

    [[nodiscard]] int GetId() const;

    // TODO: MOVE THESE TO VISITOR
    // // Expressions that haven't been decomposed
    // std::deque<std::unique_ptr<LogicExpression>> expressions;
    //
    // // Atomics or their negations
    // std::vector<std::unique_ptr<LogicExpression>> env;

    // Parent pointer to get what it decomposes from
    std::unique_ptr<TreeNode> parent;

    protected:
    static int globalId;

    private:
    // Id for building Willow graph
    int id;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_TREENODE_HPP

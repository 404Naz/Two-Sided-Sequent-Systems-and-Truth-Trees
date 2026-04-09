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

    virtual std::any Accept(TreeVisitor& visitor) const = 0;

    void SetParent(TreeNode& node);
    [[nodiscard]] bool HasParent() const;

    [[nodiscard]] int GetId() const;

    // Parent pointer to get what it decomposes from
    TreeNode* parent = nullptr;

    protected:
    static int globalId;

    private:
    // Id for building Willow graph
    int id;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_TREENODE_HPP

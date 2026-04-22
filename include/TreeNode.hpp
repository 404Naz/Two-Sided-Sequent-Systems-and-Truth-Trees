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

    virtual bool AddNode(std::unique_ptr<TreeNode> node) = 0;
    virtual std::unique_ptr<LogicExpression> GetStatement() = 0;

    // Parent pointer to get what it decomposes from
    TreeNode* parent = nullptr;
    bool isPremise{};

    protected:
    static int globalId;

    private:
    // Id for building Willow graph
    int id;
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_TREENODE_HPP

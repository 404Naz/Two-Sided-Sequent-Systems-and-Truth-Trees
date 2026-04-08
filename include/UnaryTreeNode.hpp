//
// Created by Andrew Nazareth on 3/27/26.
//

#ifndef SEQUENTSYSTEMSTOTRUTHTREES_UNARYTREENODE_HPP
#define SEQUENTSYSTEMSTOTRUTHTREES_UNARYTREENODE_HPP
#include "TreeNode.hpp"

namespace Logic_Project {
class UnaryTreeNode final : public TreeNode {
    public:
    UnaryTreeNode() = default;
    UnaryTreeNode(const UnaryTreeNode& other);
    explicit UnaryTreeNode(const LogicExpression& statement);
    UnaryTreeNode(const LogicExpression& statement, TreeNode& parent);
    UnaryTreeNode(const LogicExpression& statement, TreeNode& parent, std::unique_ptr<TreeNode> child);

    std::any Accept(TreeVisitor& visitor) const override;
    [[nodiscard]] std::unique_ptr<TreeNode> Copy() const override;

    [[nodiscard]] bool HasChild() const;
    void SetChild(std::unique_ptr<TreeNode> newChild);

    // Statement represented by this node
    std::unique_ptr<LogicExpression> statement;
    std::unique_ptr<TreeNode> child;
    int decomposition1{};
    bool isPremise{};
};
}

#endif // SEQUENTSYSTEMSTOTRUTHTREES_UNARYTREENODE_HPP

//
// Created by Andrew Nazareth on 4/2/26.
//

#include "SequentConverter.hpp"

#include "BinarySequentNode.hpp"
#include "ClosedTreeNode.hpp"
#include "UnarySequentNode.hpp"

#include <RecursiveCast.hpp>
#include <BinaryTreeNode.hpp>
#include <LogicalAtom.hpp>
#include <LogicalNot.hpp>
#include <UnaryTreeNode.hpp>
#include <iostream>
#include <ostream>

namespace Logic_Project {

std::any SequentConverter::VisitOld(const UnarySequentNode& node)
{
    std::cout << static_cast<int>(node.rule) << std::endl;
    int decomposition1=-1;
    int decomposition2=-1;
    int parentId = -1;
    std::unique_ptr<TreeNode> treeNode;
    std::unique_ptr<UnaryTreeNode> treeNodeP;
    std::unique_ptr<UnaryTreeNode> treeNodeC1;
    std::unique_ptr<UnaryTreeNode> treeNodeC2;
    std::unique_ptr<LogicExpression> diffP;
    std::unique_ptr<LogicExpression> diffC;
    std::vector<std::unique_ptr<LogicExpression>> diffsC;
    bool foundMatch = false;
    switch (SequentNodeRule rule = node.rule) {
    case SequentNodeRule::None:
        // A => A
        // Ideally just one thing on the left and one on the right
        for (auto& eq: node.succedents) {
            foundMatch = false;
            for (auto& eq2 : node.antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                    diffC = eq2->Copy();
                    break;
                }
            }
            if (foundMatch) {
                break;
            }
        }
        if (!foundMatch) {
            std::cerr<<"Wrong rule for node."<<std::endl;
            break;
        }

        treeNode = std::make_unique<ClosedTreeNode>();
        // search existing nodes for A, ~A statements to set as decomposition, use the second as the parent
        diffP = LogicalNot{*diffC->Copy()}.Generalize();
        for (const auto & createdNode : createdNodes) {
            auto& closed = dynamic_cast<ClosedTreeNode&>(*treeNode);
            if (createdNode->GetStatement()->Equals(*diffP) || createdNode->GetStatement()->Equals(*diffC)) {
                if (decomposition1 == -1) {
                    decomposition1 = createdNode->GetId();
                    closed.decomposition1 = decomposition1;
                } else if (decomposition2 == -1) {
                    decomposition2 = createdNode->GetId();
                    closed.decomposition2 = decomposition2;
                }
            }
        }

        // set parent as second of the contradicting things
        for (const auto & createdNode : createdNodes) {
            if (createdNode->GetId() == decomposition2) {
                parentId = createdNode->GetId();
                break;
            }
        }

        // push into list
        createdNodes.emplace_back(std::move(treeNode));
        createdNodesParentIds.emplace_back(parentId);
        break;
    case SequentNodeRule::ConjL:
        for (auto& eq: node.antecedents) {
            foundMatch = false;
            for (auto& eq2 : node.parent->antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffP = eq->Copy();
                break;
            }
        }

        // there should be 2 differences
        for (auto& eq: node.parent->antecedents) {
            foundMatch = false;
            for (auto& eq2 : node.antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffsC.push_back(eq->Copy());
            }
        }

        treeNodeP = std::make_unique<UnaryTreeNode>(*diffP);

        treeNodeC1 = std::make_unique<UnaryTreeNode>(*diffsC[0]);
        treeNodeC1->antecedent = treeNodeP->GetId();
        treeNodeC2 = std::make_unique<UnaryTreeNode>(*diffsC[1]);
        treeNodeC2->antecedent = treeNodeP->GetId();
        treeNodeP->decomposition1 = treeNodeC1->GetId();
        treeNodeP->decomposition2 = treeNodeC2->GetId();

        if (node.child == nullptr) {
            treeNodeP->isPremise = true;
        }
        if (!createdNodes.empty()) {
            parentId = createdNodes.back()->GetId();
            createdNodesParentIds.emplace_back(parentId);
        } else {
            createdNodesParentIds.emplace_back(-1);
        }

        createdNodesParentIds.emplace_back(treeNodeP->GetId());
        createdNodesParentIds.emplace_back(treeNodeC1->GetId());
        createdNodes.emplace_back(std::move(treeNodeP));
        createdNodes.emplace_back(std::move(treeNodeC1));
        createdNodes.emplace_back(std::move(treeNodeC2));
        break;
    case SequentNodeRule::DisjR:
        for (auto& eq: node.succedents) {
            foundMatch = false;
            for (auto& eq2 : node.parent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffP = LogicalNot{*eq->Copy()}.Generalize();
                break;
            }
        }

        // there should be 2 differences
        for (auto& eq: node.parent->succedents) {
            foundMatch = false;
            for (auto& eq2 : node.succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffsC.push_back(LogicalNot{*eq->Copy()}.Generalize());
            }
        }

        treeNodeP = std::make_unique<UnaryTreeNode>(*diffP);

        treeNodeC1 = std::make_unique<UnaryTreeNode>(*diffsC[0]);
        treeNodeC1->antecedent = treeNodeP->GetId();
        treeNodeC2 = std::make_unique<UnaryTreeNode>(*diffsC[1]);
        treeNodeC2->antecedent = treeNodeP->GetId();
        treeNodeP->decomposition1 = treeNodeC1->GetId();
        treeNodeP->decomposition2 = treeNodeC2->GetId();

        if (node.child == nullptr) {
            treeNodeP->isPremise = true;
        }
        if (!createdNodes.empty()) {
            parentId = createdNodes.back()->GetId();
            createdNodesParentIds.emplace_back(parentId);
        } else {
            createdNodesParentIds.emplace_back(-1);
        }

        createdNodesParentIds.emplace_back(treeNodeP->GetId());
        createdNodesParentIds.emplace_back(treeNodeC1->GetId());
        createdNodes.emplace_back(std::move(treeNodeP));
        createdNodes.emplace_back(std::move(treeNodeC1));
        createdNodes.emplace_back(std::move(treeNodeC2));
        break;
    case SequentNodeRule::ImplR:
        for (auto& eq: node.succedents) {
            foundMatch = false;
            for (auto& eq2 : node.parent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffP = LogicalNot{*eq->Copy()}.Generalize();
                break;
            }
        }

        // there should be 2 differences, one in each
        for (auto& eq: node.parent->antecedents) {
            foundMatch = false;
            for (auto& eq2 : node.antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffsC.push_back(eq->Copy());
                break;
            }
        }

        for (auto& eq: node.parent->succedents) {
            foundMatch = false;
            for (auto& eq2 : node.succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffsC.push_back(LogicalNot{*eq->Copy()}.Generalize());
                break;
            }
        }

        treeNodeP = std::make_unique<UnaryTreeNode>(*diffP);

        treeNodeC1 = std::make_unique<UnaryTreeNode>(*diffsC[0]);
        treeNodeC1->antecedent = treeNodeP->GetId();
        treeNodeC2 = std::make_unique<UnaryTreeNode>(*diffsC[1]);
        treeNodeC2->antecedent = treeNodeP->GetId();
        treeNodeP->decomposition1 = treeNodeC1->GetId();
        treeNodeP->decomposition2 = treeNodeC2->GetId();

        if (node.child == nullptr) {
            treeNodeP->isPremise = true;
        }
        if (!createdNodes.empty()) {
            parentId = createdNodes.back()->GetId();
            createdNodesParentIds.emplace_back(parentId);
        } else {
            createdNodesParentIds.emplace_back(-1);
        }

        createdNodesParentIds.emplace_back(treeNodeP->GetId());
        createdNodesParentIds.emplace_back(treeNodeC1->GetId());
        createdNodes.emplace_back(std::move(treeNodeP));
        createdNodes.emplace_back(std::move(treeNodeC1));
        createdNodes.emplace_back(std::move(treeNodeC2));
        break;
    case SequentNodeRule::NegL:
    case SequentNodeRule::NegR:
    case SequentNodeRule::WL:
    case SequentNodeRule::WR:
    case SequentNodeRule::XL:
    case SequentNodeRule::XR:
    case SequentNodeRule::CL:
    case SequentNodeRule::CR:
        // these cases should all be ignored in a tree
        break;
    default:
        std::cerr<<"Unknown rule or wrong rule for type of node."<<std::endl;
        return nullptr;
    }

    if (node.parent != nullptr && node.rule != SequentNodeRule::None) {
        node.parent->Accept(*this);
    }
    return nullptr;
}
std::any SequentConverter::VisitOld(const BinarySequentNode& node)
{
    std::unique_ptr<LogicExpression> diffL;
    std::unique_ptr<LogicExpression> diffR;
    std::unique_ptr<LogicExpression> diffP;
    std::unique_ptr<BinaryTreeNode> treeNodeP;
    std::unique_ptr<UnaryTreeNode> treeNodeL;
    std::unique_ptr<UnaryTreeNode> treeNodeR;
    UnaryTreeNode* unary;
    BinaryTreeNode* binary;
    int parentId = -1;
    switch (node.rule) {
    case SequentNodeRule::ConjR:
        if (node.child == nullptr) {
            // last statement, statements are premises
            for (int i = 0; i < node.antecedents.size(); i++) {
                std::unique_ptr<TreeNode> treeNode = std::make_unique<UnaryTreeNode>(*(node.antecedents[i]));
                parentId = -1;
                if (i > 0) {
                    parentId = createdNodes.at(i-1)->GetId();
                }
                createdNodes.emplace_back(std::move(treeNode));
                createdNodesParentIds.emplace_back(parentId);
            }
            for (auto& statement : createdNodes) {
                statement->isPremise = true;
            }
        }
        // Find succedent differences
        // there should only be one difference in each
        for (auto& eq: node.succedents) {
            bool foundMatch = false;
            for (auto& eq2 : node.leftParent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : node.rightParent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffP = eq->Copy();
                break;
            }
        }

        for (auto& eq: node.leftParent->succedents) {
            bool foundMatch = false;
            for (auto& eq2 : node.succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : node.rightParent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffL = LogicalNot{*eq->Copy()}.Generalize();
                break;
            }
        }

        for (auto& eq: node.rightParent->succedents) {
            bool foundMatch = false;
            for (auto& eq2 : node.leftParent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : node.succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffR = LogicalNot{*eq->Copy()}.Generalize();
                break;
            }
        }

        // create nodes, set the parent id (but not the pointer)
        treeNodeP = std::make_unique<BinaryTreeNode>(LogicalNot{*diffP});
        parentId = -1;
        if (!createdNodes.empty()) {
            parentId = createdNodes.back()->GetId();
        }
        if (node.isRoot) {
            treeNodeP->isPremise = true;
        }
        // Currently does not allow for stacked binary expressions (ie (A and B) and (C and D))
        createdNodesParentIds.emplace_back(parentId);
        treeNodeL = std::make_unique<UnaryTreeNode>(*diffL);
        parentId = treeNodeP->GetId();
        treeNodeL->antecedent = parentId;
        treeNodeR = std::make_unique<UnaryTreeNode>(*diffR);
        treeNodeR->antecedent = parentId;
        treeNodeP->decomposition1 = treeNodeL->GetId();
        treeNodeP->decomposition2 = treeNodeR->GetId();
        createdNodes.emplace_back(std::move(treeNodeP));
        createdNodes.emplace_back(std::move(treeNodeL));
        createdNodes.emplace_back(std::move(treeNodeR));
        createdNodesParentIds.emplace_back(parentId);
        createdNodesParentIds.emplace_back(parentId);
        break;
    case SequentNodeRule::DisjL:
        if (node.child == nullptr) {
            // last statement, statements are premises
            for (int i = 0; i < node.succedents.size(); i++) {
                std::unique_ptr<TreeNode> treeNode = std::make_unique<UnaryTreeNode>(*(node.succedents[i]));
                parentId = -1;
                if (i > 0) {
                    parentId = createdNodes.at(i-1)->GetId();
                }
                createdNodes.emplace_back(std::move(treeNode));
                createdNodesParentIds.emplace_back(parentId);
            }
            for (auto& statement : createdNodes) {
                statement->isPremise = true;
            }
        }

        // Find differences
        for (auto& eq: node.antecedents) {
            bool foundMatch = false;
            for (auto& eq2 : node.leftParent->antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : node.rightParent->antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffP = eq->Copy();
                break;
            }
        }

        for (auto& eq: node.leftParent->antecedents) {
            bool foundMatch = false;
            for (auto& eq2 : node.antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : node.rightParent->antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffL = eq->Copy();
                break;
            }
        }

        for (auto& eq: node.rightParent->antecedents) {
            bool foundMatch = false;
            for (auto& eq2 : node.leftParent->antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : node.antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffR = eq->Copy();
                break;
            }
        }

        // create nodes, set the parent id (but not the pointer)
        treeNodeP = std::make_unique<BinaryTreeNode>(*diffP);
        parentId = -1;
        if (!createdNodes.empty()) {
            parentId = createdNodes.back()->GetId();
        }
        if (node.isRoot) {
            treeNodeP->isPremise = true;
        }
        // Currently does not allow for stacked binary expressions (ie (A op B) or (C op D))
        createdNodesParentIds.emplace_back(parentId);
        treeNodeL = std::make_unique<UnaryTreeNode>(*diffL);
        parentId = treeNodeP->GetId();
        treeNodeL->antecedent = parentId;
        treeNodeR = std::make_unique<UnaryTreeNode>(*diffR);
        treeNodeR->antecedent = parentId;
        treeNodeP->decomposition1 = treeNodeL->GetId();
        treeNodeP->decomposition2 = treeNodeR->GetId();
        createdNodes.emplace_back(std::move(treeNodeP));
        createdNodes.emplace_back(std::move(treeNodeL));
        createdNodes.emplace_back(std::move(treeNodeR));
        createdNodesParentIds.emplace_back(parentId);
        createdNodesParentIds.emplace_back(parentId);
        break;
    case SequentNodeRule::ImplL:
        if (node.child == nullptr) {
            // last statement, statements are premises
            for (int i = 0; i < node.succedents.size(); i++) {
                std::unique_ptr<TreeNode> treeNode = std::make_unique<UnaryTreeNode>(*(node.succedents[i]));
                parentId = -1;
                if (i > 0) {
                    parentId = createdNodes.at(i-1)->GetId();
                }
                createdNodes.emplace_back(std::move(treeNode));
                createdNodesParentIds.emplace_back(parentId);
            }
            for (auto& statement : createdNodes) {
                statement->isPremise = true;
            }
        }

        // Find differences
        for (auto& eq: node.antecedents) {
            bool foundMatch = false;
            for (auto& eq2 : node.leftParent->antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : node.rightParent->antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffP = eq->Copy();
                break;
            }
        }

        for (auto& eq: node.leftParent->succedents) {
            bool foundMatch = false;
            for (auto& eq2 : node.succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : node.rightParent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffL = LogicalNot{*eq->Copy()}.Generalize();
                break;
            }
        }

        for (auto& eq: node.rightParent->antecedents) {
            bool foundMatch = false;
            for (auto& eq2 : node.leftParent->antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : node.antecedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffR = eq->Copy();
                break;
            }
        }

        // create nodes, set the parent id (but not the pointer)
        treeNodeP = std::make_unique<BinaryTreeNode>(*diffP);
        parentId = -1;
        if (!createdNodes.empty()) {
            parentId = createdNodes.back()->GetId();
        }
        if (node.isRoot) {
            treeNodeP->isPremise = true;
        }
        // Currently does not allow for stacked binary expressions (ie (A op B) \to (C op D))
        createdNodesParentIds.emplace_back(parentId);
        treeNodeL = std::make_unique<UnaryTreeNode>(*diffL);
        parentId = treeNodeP->GetId();
        treeNodeL->antecedent = parentId;
        treeNodeR = std::make_unique<UnaryTreeNode>(*diffR);
        treeNodeR->antecedent = parentId;
        treeNodeP->decomposition1 = treeNodeL->GetId();
        treeNodeP->decomposition2 = treeNodeR->GetId();
        createdNodes.emplace_back(std::move(treeNodeP));
        createdNodes.emplace_back(std::move(treeNodeL));
        createdNodes.emplace_back(std::move(treeNodeR));
        createdNodesParentIds.emplace_back(parentId);
        createdNodesParentIds.emplace_back(parentId);
        break;
    default:
        std::cerr<<"Unknown rule or wrong rule for type of node."<<std::endl;
        return nullptr;
    }

    if (node.leftParent != nullptr) {
        node.leftParent->Accept(*this);
    }
    if (node.rightParent != nullptr) {
        node.rightParent->Accept(*this);
    }
    return nullptr;
}

std::unique_ptr<TreeNode> SequentConverter::Visit(const UnarySequentNode& unary)
{
    std::unique_ptr<TreeNode> childTreeNode;
    if (unary.parent != nullptr && unary.rule != SequentNodeRule::None) {
        childTreeNode = std::move(unary.parent->Accept(*this));
    }
    std::unique_ptr<LogicExpression> diffC;
    std::unique_ptr<LogicExpression> diffP;

    if (unary.rule == SequentNodeRule::None) {
        std::unique_ptr<LogicExpression> diffN;
        std::unique_ptr<LogicExpression> diffDN;
        bool foundMatch = false;
        // Should be able to find pair on left and right
        for (auto& eq: unary.succedents) {
            foundMatch = false;
            for (auto& eq2 : unary.antecedents) {
                if (eq->Equals(*eq2)) {
                    // search premises
                    for (auto& p : premises) {
                        if (p->Equals(*eq2)) {
                            foundMatch = true;
                            break;
                        }
                    }

                    if (foundMatch) {
                        diffP = eq2->Copy();
                        diffN = LogicalNot{*eq2->Generalize()}.Generalize();
                    } else {
                        diffP = LogicalNot{*eq->Copy()}.Generalize();
                        diffN = LogicalNot{LogicalNot{*eq->Generalize()}}.Generalize();
                    }

                    assert(!diffN->Equals(*eq2)); // should not call copy constructor, if it does, then I need to check and catch that
                    // normal is first, premise is second, if DN, connect X to remove before
                    auto premise = std::make_unique<UnaryTreeNode>(*diffP);
                    premise->isPremise = true;
                    auto negated = std::make_unique<UnaryTreeNode>(*diffN);
                    for (const auto& p : premises) {
                        if (p->Equals(*diffN)) {
                            negated->isPremise = true;
                        }
                    }
                    auto closed = std::make_unique<ClosedTreeNode>();
                    if (auto DN = RecursiveCast<LogicalNot<LogicalNot<LogicExpression>>>(*diffN); diffDN != nullptr) {
                        diffDN = DN->GetOperand().GetOperand().Copy();
                        auto dn = std::make_unique<UnaryTreeNode>(*diffDN);
                        dn->SetAntecedent(negated->GetId());
                        closed->decomposition1 = dn->GetId();
                        closed->decomposition2 = premise->GetId();
                        premise->SetChild(std::move(closed));
                        dn->SetChild(std::move(premise));
                        negated->SetChild(std::move(dn));
                        return std::move(negated);
                    } else {
                        closed->decomposition1 = premise->GetId();
                        closed->decomposition2 = negated->GetId();
                        premise->SetChild(std::move(closed));
                        negated->SetChild(std::move(premise));
                        return std::move(negated);
                    }
                }
            }
        }
        std::cerr<<"Wrong rule for node."<<std::endl;
        return nullptr;
    } else if (unary.rule == SequentNodeRule::ConjL) {
        fprintf(stderr, "ConjL not implemented\n");
        return nullptr;
    } else if (unary.rule == SequentNodeRule::DisjR) {
        fprintf(stderr, "DisjR not implemented\n");
        return nullptr;
    } else if (unary.rule == SequentNodeRule::ImplR) {
        fprintf(stderr, "ImplR not implemented\n");
        return nullptr;
    } else if (unary.rule == SequentNodeRule::NegL || unary.rule == SequentNodeRule::NegR ||
        unary.rule == SequentNodeRule::WL || unary.rule == SequentNodeRule::WR ||
        unary.rule == SequentNodeRule::CL || unary.rule == SequentNodeRule::CR ||
        unary.rule == SequentNodeRule::XL || unary.rule == SequentNodeRule::XR) {
        // valid rules, but change nothing.
        return childTreeNode;
    }

    fprintf(stderr, "Wrong rule for node type.");
    return nullptr;
}
std::unique_ptr<TreeNode> SequentConverter::Visit(const BinarySequentNode& binary)
{
    std::unique_ptr<TreeNode> leftChildTreeNode, rightChildTreeNode;
    if (binary.leftParent != nullptr) {
        leftChildTreeNode = std::move(binary.leftParent->Accept(*this));
    }
    if (binary.rightParent != nullptr) {
        rightChildTreeNode = std::move(binary.rightParent->Accept(*this));
    }

    std::unique_ptr<LogicExpression> diffL;
    std::unique_ptr<LogicExpression> diffR;
    std::unique_ptr<LogicExpression> diffP;

    if (binary.rule == SequentNodeRule::ConjR) {
        for (auto& eq: binary.succedents) {
            bool foundMatch = false;
            for (auto& eq2 : binary.leftParent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : binary.rightParent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffP = LogicalNot{*eq->Copy()}.Generalize();
                break;
            }
        }

        for (auto& eq: binary.leftParent->succedents) {
            bool foundMatch = false;
            for (auto& eq2 : binary.succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : binary.rightParent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffL = LogicalNot{*eq->Copy()}.Generalize();
                break;
            }
        }

        for (auto& eq: binary.rightParent->succedents) {
            bool foundMatch = false;
            for (auto& eq2 : binary.leftParent->succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            for (auto& eq2 : binary.succedents) {
                if (eq->Equals(*eq2)) {
                    foundMatch = true;
                }
            }
            if (!foundMatch) {
                diffR = LogicalNot{*eq->Copy()}.Generalize();
                break;
            }
        }
        auto treeNodeP = std::make_unique<BinaryTreeNode>(*diffP);
        if (leftChildTreeNode != nullptr && leftChildTreeNode->GetStatement() != nullptr) {
            leftChildTreeNode->SetAntecedent(treeNodeP->GetId());
            if (auto statement = leftChildTreeNode->GetStatement(); statement->Equals(*diffP)) {
                treeNodeP->SetLeftChild(std::move(leftChildTreeNode));
            }
        } else {
            auto treeNodeL = std::make_unique<UnaryTreeNode>(*diffL);
            treeNodeL->antecedent = treeNodeP->GetId();
            treeNodeL->SetChild(std::move(leftChildTreeNode));
        }
        if (rightChildTreeNode != nullptr && rightChildTreeNode->GetStatement() != nullptr) {
            rightChildTreeNode->SetAntecedent(treeNodeP->GetId());
            if (auto statement = rightChildTreeNode->GetStatement(); statement->Equals(*diffR)) {
                treeNodeP->SetRightChild(std::move(rightChildTreeNode));
            }
        } else {
            auto treeNodeR = std::make_unique<UnaryTreeNode>(*diffR);
            treeNodeR->antecedent = treeNodeP->GetId();
            treeNodeR->SetChild(std::move(rightChildTreeNode));
        }
        return treeNodeP;
    } else if (binary.rule == SequentNodeRule::DisjL) {
        fprintf(stderr, "DisjL not implemented\n");
        return nullptr;
    } else if (binary.rule == SequentNodeRule::ImplL) {
        fprintf(stderr, "ImplL not implemented\n");
        return nullptr;
    }
    fprintf(stderr, "Incorrect node type for rule provided.\n");
    return nullptr;
}

std::unique_ptr<TreeNode> SequentConverter::ConvertToTree(SequentNode& node)
{
    // createdNodes.clear();
    // createdNodesParentIds.clear();
    for (const auto& a: node.antecedents) {
        premises.emplace_back(a->Copy());
    }
    for (const auto& s: node.succedents) {
        if (auto neg = RecursiveCast<LogicalNot<>>(*s); neg != nullptr) {
            premises.emplace_back(neg->GetOperand().Copy());
        } else {
            premises.emplace_back(LogicalNot{*s->Copy()}.Generalize());
        }

    }
    return node.Accept(*this);
    // if (createdNodes.size() < 2) {
    //     return nullptr;
    // }
    // for (size_t i = createdNodes.size()-1; i > 0; i--) {
    //     int parentId = createdNodesParentIds[i];
    //     auto& treeNode = createdNodes[i];
    //     size_t j = 0;
    //     for (j = i-1; j > 0; j--) {
    //         if (createdNodes[j] != nullptr && createdNodes[j]->GetId() == parentId) {
    //             break;
    //         }
    //     }
    //     if (bool ret = createdNodes[j]->AddNode(std::move(treeNode)); !ret) {
    //         std::cerr<<"Tried to add child to full node."<<std::endl;
    //     }
    // }
    //
    // return std::move(createdNodes.at(0));
}
}
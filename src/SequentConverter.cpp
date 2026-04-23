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

std::any SequentConverter::Visit(const UnarySequentNode& node)
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
std::any SequentConverter::Visit(const BinarySequentNode& node)
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
        if (node.child == nullptr) {break;}
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
                diffR = LogicalNot{*eq->Copy()}.Generalize();
                break;
            }
        }

        break;
    case SequentNodeRule::ImplL:
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
std::unique_ptr<TreeNode> SequentConverter::ConvertToTree(SequentNode& node)
{
    createdNodes.clear();
    createdNodesParentIds.clear();
    node.Accept(*this);
    for (size_t i = createdNodes.size()-1; i > 0; i--) {
        int parentId = createdNodesParentIds[i];
        auto& treeNode = createdNodes[i];
        size_t j = 0;
        for (j = i-1; j > 0; j--) {
            if (createdNodes[j] != nullptr && createdNodes[j]->GetId() == parentId) {
                break;
            }
        }
        if (bool ret = createdNodes[j]->AddNode(std::move(treeNode)); !ret) {
            std::cerr<<"Tried to add child to full node."<<std::endl;
        }
    }

    return std::move(createdNodes.at(0));
}
}
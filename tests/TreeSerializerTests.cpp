//
// Created by Andrew Nazareth on 4/3/26.
//

#include "BinaryTreeNode.hpp"
#include "ClosedTreeNode.hpp"
#include "OpenTreeNode.hpp"
#include "TreeSerializer.hpp"
#include "UnaryTreeNode.hpp"
#include "LogicalAnd.hpp"
#include "LogicalOr.hpp"
#include "LogicalNot.hpp"
#include "LogicalAtom.hpp"

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <print>
#include <regex>

namespace Logic_Project {
TEST_CASE("Open Node Serialized", "[OpenTreeNode][Serializer]")
{
    TreeSerializer treeSerializer{};
    OpenTreeNode parent; // type doesn't matter here
    OpenTreeNode node;

    node.SetParent(parent);
    auto serialized = std::any_cast<std::string>(node.Accept(treeSerializer));
    std::regex pattern{
        R"("id":\d+,\s*"text":"◯",\s*"children":\[\],\s*"decomposition":\[\],\s*"parent":\d+)"
    };

    // std::regex pattern{"id":\d+,"text":"◯","children":\[],"decomposition":\[],"parent":\d+};
    CAPTURE(serialized);
    REQUIRE(std::regex_search(serialized, pattern));
}

TEST_CASE("Closed Node Serialized", "[ClosedTreeNode][Serializer]")
{
    TreeSerializer treeSerializer{};
    ClosedTreeNode parent; // type doesn't matter here
    ClosedTreeNode node;

    node.SetParent(parent);
    auto serialized = std::any_cast<std::string>(node.Accept(treeSerializer));
    std::regex pattern{
        R"("id":\d+,\s*"text":"×",\s*"children":\[\],\s*"decomposition":\[\d+,\d+\],\s*"parent":\d+)"
    };

    // std::regex pattern{"id":\d+,"text":"◯","children":\[],"decomposition":\[],"parent":\d+};
    CAPTURE(serialized);
    REQUIRE(std::regex_search(serialized, pattern));
}

TEST_CASE("Unary Node Serialized", "[UnaryTreeNode][Serializer]")
{
    const auto expr = LogicalAtom{"B"};
    TreeSerializer treeSerializer{};
    UnaryTreeNode parent{};
    UnaryTreeNode node{expr};
    node.antecedent = parent.GetId();
    node.SetParent(parent);
    auto serialized = std::any_cast<std::string>(node.Accept(treeSerializer));
    std::regex pattern{
        R"("id":\d+,\s*"text":"B",\s*"children":\[\],\s*"decomposition":\[\],\s*"parent":\d+,"antecedent":\d+,\s*"premise":false)"
    };

    CAPTURE(serialized);
    REQUIRE(std::regex_search(serialized, pattern));
}

TEST_CASE("Binary Node Serialized", "[BinaryTreeNode][Serializer]")
{
    const auto expr = LogicalAnd<LogicalAtom>{ LogicalAtom{"B"}, LogicalAtom{"C"}};
    TreeSerializer treeSerializer{};
    UnaryTreeNode parent{};
    BinaryTreeNode node{expr, parent};
    node.isPremise = true;
    auto serialized = std::any_cast<std::string>(node.Accept(treeSerializer));
    std::regex pattern{
        R"del("id":\d+,\s*"text":"\(B ∧ C\)",\s*"children":\[\],\s*"decomposition":\[\],\s*"parent":\d+,\s*"premise":true)del"
    };

    CAPTURE(serialized);
    REQUIRE(std::regex_search(serialized, pattern));
}

TEST_CASE("Tree Serialized", "[TreeNode][Serializer]")
{
    TreeSerializer treeSerializer {};
    UnaryTreeNode B{LogicalAtom{"B"}};
    B.isPremise = true;
    auto NegB = std::make_unique<UnaryTreeNode>(LogicalNot{LogicalAtom{"B"}});
    NegB->isPremise = true;
    auto c = std::make_unique<ClosedTreeNode>();
    c->decomposition1 = B.GetId();
    c->decomposition2 = NegB->GetId();
    NegB->SetChild(std::move(c));
    B.SetChild(std::move(NegB));

    std::string result = treeSerializer.Serialize(B);

    std::regex pattern {
        R"aa(\{"nodes":\[\{"id":\d+,"text":"B","children":\[\d+\],"decomposition":\[\],"premise":true\},\{"id":\d+,"text":"¬B","children":\[\d+\],"decomposition":\[\],"parent":\d+,"premise":true\},\{"id":\d+,"text":"×","children":\[\],"decomposition":\[\d+,\d+\],"parent":\d+\}\],"options":\{"requireAtomicContradiction":true,"requireAllBranchesTerminated":true,"lockedOptions":false\}\})aa"
    };
    // Should roughly match test2.willow
    CAPTURE(result);
    REQUIRE(std::regex_search(result, pattern));
}
}

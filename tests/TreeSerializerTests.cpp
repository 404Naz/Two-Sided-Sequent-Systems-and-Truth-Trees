//
// Created by Andrew Nazareth on 4/3/26.
//

#include "BinaryTreeNode.hpp"
#include "ClosedTreeNode.hpp"
#include "OpenTreeNode.hpp"
#include "TreeSerializer.hpp"
#include "UnaryTreeNode.hpp"
#include <catch2/catch_test_macros.hpp>
#include <regex>

namespace Logic_Project {
TEST_CASE("Open Node Serialized", "[OpenTreeNode][Serializer]")
{
    TreeSerializer treeSerializer{};
    const OpenTreeNode parent; // type doesn't matter here
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
    const ClosedTreeNode parent; // type doesn't matter here
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
}

//
// Created by Andrew Nazareth on 4/2/26.
//

#include "TreeNode.hpp"
#include "UnaryTreeNode.hpp"
#include "BinaryTreeNode.hpp"
#include "ClosedTreeNode.hpp"
#include "OpenTreeNode.hpp"
#include <catch2/catch_test_macros.hpp>

namespace Logic_Project {
TEST_CASE("Different Node ID", "[Constructor][Id][TreeNode]")
{
    const UnaryTreeNode u{};
    const BinaryTreeNode b{};
    const ClosedTreeNode c{};
    const OpenTreeNode o{};

    REQUIRE(u.GetId() != b.GetId());
    REQUIRE(u.GetId() != o.GetId());
    REQUIRE(u.GetId() != c.GetId());
    REQUIRE(b.GetId() != c.GetId());
    REQUIRE(b.GetId() != o.GetId());
    REQUIRE(c.GetId() != o.GetId());
}



}
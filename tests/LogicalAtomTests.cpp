//
// Created by Andrew Nazareth on 3/22/26.
//

#include "LogicalAtom.hpp"
#include <catch2/catch_test_macros.hpp>

namespace Logic_Project{

TEST_CASE("Representation Equality", "[Atomic][Equality]")
{
    LogicalAtom A{"A"};
    LogicalAtom A2{"A"};
    LogicalAtom B{"B"};

    REQUIRE(A.Equals(A2));
    REQUIRE(A2.Equals(A));
    REQUIRE(!B.Equals(A));
    REQUIRE(!A.Equals(B));
}
}
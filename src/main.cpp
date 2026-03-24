//
// Created by Andrew Nazareth on 3/20/26.
//
#include <cstdlib>
#include <print>
#include "BinaryLogicExpression.hpp"
#include "LeafLogicExpression.hpp"
#include "UnaryLogicExpression.hpp"
#include "LogicalAtom.hpp"
#include "LogicalAnd.hpp"
#include "LogicalNot.hpp"

int main(int argc, char **argv) {
    Logic_Project::LogicalAtom A{"A"};
    Logic_Project::LogicalAtom A2{"A"};
    Logic_Project::LogicalAtom B{"B"};
    std::print("MAIN {}", argc);

    std::print("{}", A.GetRepresentation());

    return EXIT_SUCCESS;
}


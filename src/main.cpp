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

#include <BinarySequentNode.hpp>
#include <SequentConverter.hpp>
#include <TreeSerializer.hpp>
#include <UnarySequentNode.hpp>

using namespace Logic_Project;

#define EX1

int main(int, char**) {
    SequentConverter converter{};
    TreeSerializer serializer{};
#ifdef EX1
    std::vector<std::unique_ptr<LogicExpression>> antecedent = {};
    antecedent.emplace_back(LogicalAtom{"B"}.Generalize());
    antecedent.emplace_back(LogicalAtom{"C"}.Generalize());
    std::vector<std::unique_ptr<LogicExpression>> succedent = {};
    succedent.emplace_back(LogicalAnd{LogicalAtom{"B"}, LogicalAtom{"C"}}.Generalize());
    BinarySequentNode BC_BandC {antecedent, succedent};
    BC_BandC.rule = SequentNodeRule::ConjR;

    // Left Branch
    succedent.clear();
    succedent.emplace_back(LogicalAtom{"B"}.Generalize());
    UnarySequentNode BC_B{antecedent, succedent};
    BC_B.rule = SequentNodeRule::XL;
    antecedent.clear();
    antecedent.emplace_back(LogicalAtom{"C"}.Generalize());
    antecedent.emplace_back(LogicalAtom{"B"}.Generalize());
    UnarySequentNode CB_B{antecedent, succedent};
    CB_B.rule = SequentNodeRule::WL;
    antecedent.clear();
    antecedent.emplace_back(LogicalAtom{"B"}.Generalize());
    UnarySequentNode B_B{antecedent, succedent};
    B_B.rule = SequentNodeRule::None;

    // Right Branch
    succedent.clear();
    antecedent.clear();
    succedent.emplace_back(LogicalAtom{"C"}.Generalize());
    antecedent.emplace_back(LogicalAtom{"B"}.Generalize());
    antecedent.emplace_back(LogicalAtom{"C"}.Generalize());
    UnarySequentNode BC_C{antecedent, succedent};
    BC_C.rule = SequentNodeRule::WL;
    antecedent.clear();
    antecedent.emplace_back(LogicalAtom{"C"}.Generalize());
    UnarySequentNode C_C{antecedent, succedent};
    C_C.rule = SequentNodeRule::None;

    // Put together
    BC_BandC.isRoot = true;
    BC_BandC.SetLeftParent(&BC_B);
    BC_BandC.SetRightParent(&BC_C);
    BC_B.SetParent(&CB_B);
    CB_B.SetParent(&B_B);
    BC_C.SetParent(&C_C);

    // Convert to tree
    auto treeRoot = converter.ConvertToTree(BC_BandC);
    std::string result = serializer.Serialize(*treeRoot);
#endif
    return EXIT_SUCCESS;
}


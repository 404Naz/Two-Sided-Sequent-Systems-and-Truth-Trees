//
// Created by Andrew Nazareth on 3/20/26.
//
#include "BinaryLogicExpression.hpp"
#include "LeafLogicExpression.hpp"
#include "LogicalAnd.hpp"
#include "LogicalAtom.hpp"
#include "LogicalConditional.hpp"
#include "LogicalNot.hpp"
#include "UnaryLogicExpression.hpp"
#include <cstdlib>
#include <print>

#include <BinarySequentNode.hpp>
#include <SequentConverter.hpp>
#include <TreeSerializer.hpp>
#include <UnarySequentNode.hpp>

using namespace Logic_Project;

// #define EX1
#define EX2
// #define EX3

int main(int, char**) {
    SequentConverter converter{};
    TreeSerializer serializer{};
    std::string result{};
    std::vector<std::unique_ptr<LogicExpression>> antecedent = {};
    std::vector<std::unique_ptr<LogicExpression>> succedent = {};
#ifdef EX1
    antecedent.clear();
    antecedent.emplace_back(LogicalAtom{"B"}.Generalize());
    antecedent.emplace_back(LogicalAtom{"C"}.Generalize());
    succedent.clear();
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
    auto treeRoot1 = converter.ConvertToTree(BC_BandC);
    result = serializer.Serialize(*treeRoot1);

    FILE* f1 = fopen("ex1.willow", "w");
    fprintf(f1, "%s\n", result.c_str());
    fclose(f1);
#endif

#ifdef EX2
    antecedent.clear();
    succedent.clear();
    antecedent.push_back(LogicalConditional{LogicalConditional{LogicalAtom{"A"}, LogicalAtom{"B"}}, LogicalAtom{"A"}}.Generalize());
    succedent.push_back(LogicalAtom{"A"}.Generalize());
    auto AtoBtoA_A = UnarySequentNode{antecedent, succedent};
    AtoBtoA_A.rule = SequentNodeRule::CL;
    AtoBtoA_A.isRoot = true;
    succedent.push_back(LogicalAtom{"A"}.Generalize());
    auto AtoBtoA_AA = BinarySequentNode(antecedent, succedent);
    AtoBtoA_AA.rule = SequentNodeRule::ImplL;

    // left
    succedent.clear();
    antecedent.clear();
    succedent.emplace_back(LogicalAtom{"A"}.Generalize());
    succedent.emplace_back(LogicalConditional{LogicalAtom{"A"}, LogicalAtom{"B"}}.Generalize());
    auto AAtoB = UnarySequentNode(antecedent, succedent);
    AAtoB.rule = SequentNodeRule::ImplR;
    succedent.clear();
    antecedent.clear();
    succedent.emplace_back(LogicalAtom{"A"}.Generalize());
    succedent.emplace_back(LogicalAtom{"B"}.Generalize());
    antecedent.emplace_back(LogicalAtom{"A"}.Generalize());
    auto A_AB = UnarySequentNode(antecedent, succedent);
    A_AB.rule = SequentNodeRule::WR;
    succedent.clear();
    antecedent.clear();
    succedent.emplace_back(LogicalAtom{"A"}.Generalize());
    antecedent.emplace_back(LogicalAtom{"A"}.Generalize());
    auto A_Al = UnarySequentNode(antecedent, succedent);
    A_Al.rule = SequentNodeRule::None;

    // right
    succedent.clear();
    antecedent.clear();
    succedent.emplace_back(LogicalAtom{"A"}.Generalize());
    antecedent.emplace_back(LogicalAtom{"A"}.Generalize());
    auto A_Ar = UnarySequentNode(antecedent, succedent);
    A_Ar.rule = SequentNodeRule::None;

    // combine
    AtoBtoA_A.SetParent(&AtoBtoA_AA);
    AtoBtoA_AA.SetRightParent(&A_Ar);
    AtoBtoA_AA.SetLeftParent(&AAtoB);
    AAtoB.SetParent(&A_AB);
    A_AB.SetParent(&A_Al);

    // Convert to tree
    auto treeRoot2 = converter.ConvertToTree(AtoBtoA_A);
    result = serializer.Serialize(*treeRoot2);

    FILE* f2 = fopen("ex2.willow", "w");
    fprintf(f2, "%s\n", result.c_str());
    fclose(f2);
#endif

#ifdef EX3
    // Convert to tree
    auto treeRoot3 = converter.ConvertToTree(BC_BandC);
    result = serializer.Serialize(*treeRoot3);

    FILE* f3 = fopen("ex3.willow", "w");
    fprintf(f3, "%s\n", result.c_str());
    fclose(f3);
#endif
    return EXIT_SUCCESS;
}


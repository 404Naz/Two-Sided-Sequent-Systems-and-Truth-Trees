//
// Created by Andrew Nazareth on 4/14/26.
//

#include "BinarySequentNode.hpp"
#include "SequentNode.hpp"
#include "UnarySequentNode.hpp"
#include "catch2/catch_test_macros.hpp"
#include "LogicalAtom.hpp"
#include "LogicalNot.hpp"
#include "LogicalAnd.hpp"
#include "LogicalOr.hpp"
#include "SequentConverter.hpp"
#include "TreeSerializer.hpp"

#include <regex>

namespace Logic_Project {
TEST_CASE("SequentTreeBuilding", "[BinarySequentNode][UnarySequentNode]")
{
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
    BC_BandC.SetLeftParent(&BC_B);
    BC_BandC.SetRightParent(&BC_C);
    REQUIRE(BC_B.child == &BC_BandC);
    REQUIRE(BC_C.child == &BC_BandC);
    BC_B.SetParent(&CB_B);
    REQUIRE(CB_B.child == &BC_B);
    CB_B.SetParent(&B_B);
    REQUIRE(B_B.child == &CB_B);
    BC_C.SetParent(&C_C);
    REQUIRE(C_C.child == &BC_C);
}

TEST_CASE("SequentConversion", "[SequentConverter][TreeNodeSerializer]")
{
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
    REQUIRE(BC_B.child == &BC_BandC);
    REQUIRE(BC_C.child == &BC_BandC);
    BC_B.SetParent(&CB_B);
    REQUIRE(CB_B.child == &BC_B);
    CB_B.SetParent(&B_B);
    REQUIRE(B_B.child == &CB_B);
    BC_C.SetParent(&C_C);
    REQUIRE(C_C.child == &BC_C);

    // Convert to tree
    SequentConverter converter{};
    TreeSerializer serializer{};
    auto treeRoot = converter.ConvertToTree(BC_BandC);
    REQUIRE(treeRoot != nullptr);
    std::string result = serializer.Serialize(*treeRoot);
    std::string expected = R"aa({"nodes":[{"id":0,"text":"B","children":[1],"decomposition":[],"premise":true},{"id":1,"text":"C","children":[2],"decomposition":[],"parent":0,"premise":true},{"id":2,"text":"¬(B ∧ C)","children":[4,3],"decomposition":[3,4],"parent":1,"premise":true},{"id":4,"text":"¬C","children":[6],"decomposition":[],"parent":2,"antecedent":2,"premise":false},{"id":6,"text":"×","children":[],"decomposition":[1,4],"parent":4},{"id":3,"text":"¬B","children":[5],"decomposition":[],"parent":2,"antecedent":2,"premise":false},{"id":5,"text":"×","children":[],"decomposition":[0,3],"parent":3}],"options":{"requireAtomicContradiction":true,"requireAllBranchesTerminated":true,"lockedOptions":false}})aa";

    CAPTURE(result);
    REQUIRE(result == expected);
}
}
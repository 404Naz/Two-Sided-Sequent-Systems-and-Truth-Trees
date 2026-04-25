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
#include "LogicalConditional.hpp"
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

TEST_CASE("SequentConversion - None", "[SequentConverter][TreeNodeSerializer]")
{
    std::vector<std::unique_ptr<LogicExpression>> antecedent = {};
    antecedent.emplace_back(LogicalAtom{"B"}.Generalize());
    std::vector<std::unique_ptr<LogicExpression>> succedent = {};
    succedent.emplace_back(LogicalAtom{"B"}.Generalize());
    UnarySequentNode B_B {antecedent, succedent};
    B_B.rule = SequentNodeRule::None;

    SequentConverter converter{};
    TreeSerializer serializer{};
    auto treeRoot = converter.ConvertToTree(B_B);
    REQUIRE(treeRoot != nullptr);
    std::string result = serializer.Serialize(*treeRoot);
    std::string expected = R"aa({"nodes":[{"id":1,"text":"¬B","children":[0],"decomposition":[],"premise":true},{"id":0,"text":"B","children":[2],"decomposition":[],"parent":1,"premise":true},{"id":2,"text":"×","children":[],"decomposition":[0,1],"parent":0}],"options":{"requireAtomicContradiction":true,"requireAllBranchesTerminated":true,"lockedOptions":false}})aa";

    CAPTURE(result);
    REQUIRE(result == expected);
}

TEST_CASE("SequentConversion - None and ConjR", "[SequentConverter][TreeNodeSerializer]")
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
    std::string expected = R"aa({"nodes":[{"id":6,"text":"¬(B ∧ C)","children":[1,4],"decomposition":[1,4],"premise":true},{"id":1,"text":"¬B","children":[0],"decomposition":[],"parent":6,"antecedent":6,"premise":false},{"id":0,"text":"B","children":[2],"decomposition":[],"parent":1,"premise":true},{"id":2,"text":"×","children":[],"decomposition":[0,1],"parent":0},{"id":4,"text":"¬C","children":[3],"decomposition":[],"parent":6,"antecedent":6,"premise":false},{"id":3,"text":"C","children":[5],"decomposition":[],"parent":4,"premise":true},{"id":5,"text":"×","children":[],"decomposition":[3,4],"parent":3}],"options":{"requireAtomicContradiction":true,"requireAllBranchesTerminated":true,"lockedOptions":false}})aa";

    CAPTURE(result);
    REQUIRE(result == expected);
}

TEST_CASE("SequentConversion - ConjL", "[SequentConverter][TreeNodeSerializer]")
{
    std::vector<std::unique_ptr<LogicExpression>> antecedent = {};
    antecedent.emplace_back(LogicalAnd { LogicalAtom { "A" }, LogicalAtom { "B" } }.Generalize());
    std::vector<std::unique_ptr<LogicExpression>> succedent = {};
    UnarySequentNode AandB_ { antecedent, succedent };
    AandB_.rule = SequentNodeRule::ConjL;

    antecedent.clear();
    antecedent.emplace_back(LogicalAtom { "A" }.Generalize());
    antecedent.emplace_back(LogicalAtom { "B" }.Generalize());
    UnarySequentNode AB_ { antecedent, succedent };
    // ignore the "Wrong rule for node" error message. I did not set the proper rule (i did not make the rest of the tree)

    AandB_.SetParent(&AB_);
    REQUIRE(AB_.child == &AandB_);

    // Convert to tree
    SequentConverter converter {};
    TreeSerializer serializer {};
    auto treeRoot = converter.ConvertToTree(AandB_);
    REQUIRE(treeRoot != nullptr);
    std::string result = serializer.Serialize(*treeRoot);
    std::string expected = R"aa({"nodes":[{"id":0,"text":"(A ∧ B)","children":[1],"decomposition":[1,2],"premise":true},{"id":1,"text":"A","children":[2],"decomposition":[],"parent":0,"antecedent":0,"premise":false},{"id":2,"text":"B","children":[],"decomposition":[],"parent":1,"antecedent":0,"premise":false}],"options":{"requireAtomicContradiction":true,"requireAllBranchesTerminated":true,"lockedOptions":false}})aa";

    CAPTURE(result);
    REQUIRE(result == expected);
}

TEST_CASE("SequentConversion - DisjR", "[SequentConverter][TreeNodeSerializer]")
{
    std::vector<std::unique_ptr<LogicExpression>> succedent = {};
    succedent.emplace_back(LogicalOr { LogicalAtom { "A" }, LogicalAtom { "B" } }.Generalize());
    std::vector<std::unique_ptr<LogicExpression>> antecedent = {};
    UnarySequentNode AorB { antecedent, succedent };
    AorB.rule = SequentNodeRule::DisjR;

    succedent.clear();
    succedent.emplace_back(LogicalAtom { "A" }.Generalize());
    succedent.emplace_back(LogicalAtom { "B" }.Generalize());
    UnarySequentNode AB { antecedent, succedent };
    // ignore the "Wrong rule for node" error message. I did not set the proper rule (i did not make the rest of the tree)

    AorB.SetParent(&AB);
    REQUIRE(AB.child == &AorB);

    // Convert to tree
    SequentConverter converter {};
    TreeSerializer serializer {};
    auto treeRoot = converter.ConvertToTree(AorB);
    REQUIRE(treeRoot != nullptr);
    std::string result = serializer.Serialize(*treeRoot);
    std::string expected = R"aa({"nodes":[{"id":0,"text":"¬(A ∨ B)","children":[1],"decomposition":[1,2],"premise":true},{"id":1,"text":"¬A","children":[2],"decomposition":[],"parent":0,"antecedent":0,"premise":false},{"id":2,"text":"¬B","children":[],"decomposition":[],"parent":1,"antecedent":0,"premise":false}],"options":{"requireAtomicContradiction":true,"requireAllBranchesTerminated":true,"lockedOptions":false}})aa";

    CAPTURE(result);
    REQUIRE(result == expected);
}

TEST_CASE("SequentConversion - ImplR", "[SequentConverter][TreeNodeSerializer]")
{
    std::vector<std::unique_ptr<LogicExpression>> succedent = {};
    succedent.emplace_back(LogicalConditional { LogicalAtom { "A" }, LogicalAtom { "B" } }.Generalize());
    std::vector<std::unique_ptr<LogicExpression>> antecedent = {};
    UnarySequentNode AtoB { antecedent, succedent };
    AtoB.rule = SequentNodeRule::ImplR;

    succedent.clear();
    antecedent.emplace_back(LogicalAtom { "A" }.Generalize());
    succedent.emplace_back(LogicalAtom { "B" }.Generalize());
    UnarySequentNode A_B { antecedent, succedent };
    // ignore the "Wrong rule for node" error message. I did not set the proper rule (i did not make the rest of the tree)

    AtoB.SetParent(&A_B);
    REQUIRE(A_B.child == &AtoB);

    // Convert to tree
    SequentConverter converter {};
    TreeSerializer serializer {};
    auto treeRoot = converter.ConvertToTree(AtoB);
    REQUIRE(treeRoot != nullptr);
    std::string result = serializer.Serialize(*treeRoot);
    std::string expected = R"aa({"nodes":[{"id":0,"text":"¬(A → B)","children":[1],"decomposition":[1,2],"premise":true},{"id":1,"text":"A","children":[2],"decomposition":[],"parent":0,"antecedent":0,"premise":false},{"id":2,"text":"¬B","children":[],"decomposition":[],"parent":1,"antecedent":0,"premise":false}],"options":{"requireAtomicContradiction":true,"requireAllBranchesTerminated":true,"lockedOptions":false}})aa";

    CAPTURE(result);
    REQUIRE(result == expected);
}

TEST_CASE("SequentConversion - ImplL", "[SequentConverter][TreeNodeSerializer]")
{
    std::vector<std::unique_ptr<LogicExpression>> antecedent = {};
    antecedent.emplace_back(LogicalConditional { LogicalAtom { "A" }, LogicalAtom { "B" } }.Generalize());
    std::vector<std::unique_ptr<LogicExpression>> succedent = {};
    BinarySequentNode AtoB { antecedent, succedent };
    AtoB.rule = SequentNodeRule::ImplL;
    AtoB.isRoot = true;

    antecedent.clear();
    succedent.emplace_back(LogicalAtom { "A" }.Generalize());
    UnarySequentNode A { antecedent, succedent };
    succedent.clear();
    antecedent.clear();
    antecedent.emplace_back(LogicalAtom { "B" }.Generalize());
    UnarySequentNode B_ { antecedent, succedent };
    // ignore the "Wrong rule for node" error message. I did not set the proper rule (i did not make the rest of the tree)

    AtoB.SetLeftParent(&A);
    AtoB.SetRightParent(&B_);
    REQUIRE(A.child == &AtoB);
    REQUIRE(B_.child == &AtoB);

    // Convert to tree
    SequentConverter converter {};
    TreeSerializer serializer {};
    auto treeRoot = converter.ConvertToTree(AtoB);
    REQUIRE(treeRoot != nullptr);
    std::string result = serializer.Serialize(*treeRoot);
    std::string expected = R"aa({"nodes":[{"id":0,"text":"(A → B)","children":[1,2],"decomposition":[1,2],"premise":true},{"id":1,"text":"¬A","children":[],"decomposition":[],"parent":0,"antecedent":0,"premise":false},{"id":2,"text":"B","children":[],"decomposition":[],"parent":0,"antecedent":0,"premise":false}],"options":{"requireAtomicContradiction":true,"requireAllBranchesTerminated":true,"lockedOptions":false}})aa";

    CAPTURE(result);
    REQUIRE(result == expected);
}

TEST_CASE("SequentConversion - DisjL", "[SequentConverter][TreeNodeSerializer]")
{
    std::vector<std::unique_ptr<LogicExpression>> antecedent = {};
    antecedent.emplace_back(LogicalOr{ LogicalAtom { "A" }, LogicalAtom { "B" } }.Generalize());
    std::vector<std::unique_ptr<LogicExpression>> succedent = {};
    BinarySequentNode AorB { antecedent, succedent };
    AorB.rule = SequentNodeRule::DisjL;
    AorB.isRoot = true;

    antecedent.clear();
    antecedent.emplace_back(LogicalAtom { "A" }.Generalize());
    UnarySequentNode A_ { antecedent, succedent };
    antecedent.clear();
    antecedent.emplace_back(LogicalAtom { "B" }.Generalize());
    UnarySequentNode B_ { antecedent, succedent };
    // ignore the "Wrong rule for node" error message. I did not set the proper rule (i did not make the rest of the tree)

    AorB.SetLeftParent(&A_);
    AorB.SetRightParent(&B_);
    REQUIRE(A_.child == &AorB);
    REQUIRE(B_.child == &AorB);

    // Convert to tree
    SequentConverter converter {};
    TreeSerializer serializer {};
    auto treeRoot = converter.ConvertToTree(AorB);
    REQUIRE(treeRoot != nullptr);
    std::string result = serializer.Serialize(*treeRoot);
    std::string expected = R"aa({"nodes":[{"id":0,"text":"(A ∨ B)","children":[1,2],"decomposition":[1,2],"premise":true},{"id":1,"text":"A","children":[],"decomposition":[],"parent":0,"antecedent":0,"premise":false},{"id":2,"text":"B","children":[],"decomposition":[],"parent":0,"antecedent":0,"premise":false}],"options":{"requireAtomicContradiction":true,"requireAllBranchesTerminated":true,"lockedOptions":false}})aa";

    CAPTURE(result);
    REQUIRE(result == expected);
}

}
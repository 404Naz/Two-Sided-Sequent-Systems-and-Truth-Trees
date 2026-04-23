//
// Tree node tests and conversion examples
//

#include "UnaryTreeNode.hpp"
#include "BinaryTreeNode.hpp"
#include "ClosedTreeNode.hpp"
#include "OpenTreeNode.hpp"
#include "TreeSerializer.hpp"

#include "LogicExpression.hpp"
#include "RecursiveCast.hpp"
#include "LogicalAtom.hpp"
#include "LogicalAnd.hpp"
#include "LogicalOr.hpp"
#include "LogicalNot.hpp"

#include <catch2/catch_test_macros.hpp>

namespace Logic_Project {

// --- Node construction and wiring ---

TEST_CASE("All node types get unique IDs", "[TreeNode][Id]")
{
    UnaryTreeNode u1{};
    UnaryTreeNode u2{};
    BinaryTreeNode b{};
    ClosedTreeNode c{};
    OpenTreeNode o{};

    REQUIRE(u1.GetId() != u2.GetId());
    REQUIRE(u1.GetId() != b.GetId());
    REQUIRE(b.GetId() != c.GetId());
    REQUIRE(c.GetId() != o.GetId());
}

TEST_CASE("UnaryTreeNode parent child wiring", "[UnaryTreeNode]")
{
    UnaryTreeNode parent{LogicalAtom{"A"}};
    REQUIRE_FALSE(parent.HasParent());
    REQUIRE_FALSE(parent.HasChild());

    auto child = std::make_unique<UnaryTreeNode>(LogicalAtom{"B"});
    parent.SetChild(std::move(child));

    REQUIRE(parent.HasChild());
    REQUIRE(parent.child->HasParent());
    REQUIRE(parent.child->parent == &parent);
}

TEST_CASE("BinaryTreeNode left right wiring", "[BinaryTreeNode]")
{
    BinaryTreeNode root{LogicalAnd<LogicalAtom>{LogicalAtom{"A"}, LogicalAtom{"B"}}};

    auto left = std::make_unique<UnaryTreeNode>(LogicalAtom{"A"});
    auto right = std::make_unique<UnaryTreeNode>(LogicalAtom{"B"});

    root.SetLeftChild(std::move(left));
    root.SetRightChild(std::move(right));

    REQUIRE(root.HasLeftChild());
    REQUIRE(root.HasRightChild());
    REQUIRE(root.left->parent == &root);
    REQUIRE(root.right->parent == &root);
}

TEST_CASE("Nodes store statements", "[TreeNode][Statement]")
{
    UnaryTreeNode u{LogicalAtom{"A"}};
    REQUIRE(u.statement->Serialize() == "A");

    BinaryTreeNode b{LogicalOr<LogicalAtom>{LogicalAtom{"A"}, LogicalAtom{"B"}}};
    REQUIRE(b.statement->Serialize() == "(A ∨ B)");
}

TEST_CASE("Default isPremise and antecedent", "[TreeNode][Defaults]")
{
    UnaryTreeNode u{LogicalAtom{"A"}};
    REQUIRE(u.isPremise == false);
    REQUIRE(u.antecedent == -1);

    BinaryTreeNode b{LogicalAtom{"A"}};
    REQUIRE(b.isPremise == false);
    REQUIRE(b.antecedent == -1);
}

// --- Serializer output ---

TEST_CASE("Serialized UnaryTreeNode has premise flag", "[Serializer]")
{
    TreeSerializer ser{};
    UnaryTreeNode node{LogicalAtom{"P"}};
    node.isPremise = true;

    auto result = std::any_cast<std::string>(node.Accept(ser));
    REQUIRE(result.find("\"text\":\"P\"") != std::string::npos);
    REQUIRE(result.find("\"premise\":true") != std::string::npos);
}

TEST_CASE("Serialized BinaryTreeNode references both children", "[Serializer]")
{
    TreeSerializer ser{};
    BinaryTreeNode root{LogicalOr<LogicalAtom>{LogicalAtom{"A"}, LogicalAtom{"B"}}};
    root.isPremise = true;

    auto left = std::make_unique<UnaryTreeNode>(LogicalAtom{"A"});
    auto right = std::make_unique<UnaryTreeNode>(LogicalAtom{"B"});
    int leftId = left->GetId();
    int rightId = right->GetId();
    root.SetLeftChild(std::move(left));
    root.SetRightChild(std::move(right));

    auto result = std::any_cast<std::string>(root.Accept(ser));
    std::string expected = std::to_string(leftId) + "," + std::to_string(rightId);
    REQUIRE(result.find("\"children\":[" + expected + "]") != std::string::npos);
}

TEST_CASE("ClosedTreeNode serializes as x with decomposition", "[Serializer]")
{
    TreeSerializer ser{};
    UnaryTreeNode parent{LogicalAtom{"A"}};
    ClosedTreeNode closed{};
    closed.SetParent(parent);
    closed.decomposition1 = 10;
    closed.decomposition2 = 20;

    auto result = std::any_cast<std::string>(closed.Accept(ser));
    REQUIRE(result.find("\"text\":\"×\"") != std::string::npos);
    REQUIRE(result.find("\"decomposition\":[10,20]") != std::string::npos);
}

TEST_CASE("OpenTreeNode serializes as circle", "[Serializer]")
{
    TreeSerializer ser{};
    UnaryTreeNode parent{LogicalAtom{"A"}};
    OpenTreeNode open{};
    open.SetParent(parent);

    auto result = std::any_cast<std::string>(open.Accept(ser));
    REQUIRE(result.find("\"text\":\"◯\"") != std::string::npos);
}

TEST_CASE("Full Serialize has nodes array and options", "[Serializer][FullTree]")
{
    TreeSerializer ser{};
    UnaryTreeNode root{LogicalAtom{"A"}};
    root.isPremise = true;
    root.SetChild(std::make_unique<OpenTreeNode>());

    std::string json = ser.Serialize(root);
    REQUIRE(json.front() == '{');
    REQUIRE(json.back() == '}');
    REQUIRE(json.find("\"nodes\":[") != std::string::npos);
    REQUIRE(json.find("\"requireAtomicContradiction\":true") != std::string::npos);
}

TEST_CASE("SerializeNodes collects right number of nodes", "[Serializer]")
{
    TreeSerializer ser{};
    UnaryTreeNode root{LogicalAtom{"A"}};
    root.isPremise = true;

    auto negA = std::make_unique<UnaryTreeNode>(LogicalNot{LogicalAtom{"A"}});
    negA->isPremise = true;
    auto closed = std::make_unique<ClosedTreeNode>();
    closed->decomposition1 = root.GetId();
    closed->decomposition2 = negA->GetId();
    negA->SetChild(std::move(closed));
    root.SetChild(std::move(negA));

    root.Accept(ser);
    REQUIRE(ser.serializedNodes.size() == 3);  // A, ¬A, ×
}

// --- Conversion examples ---
// These build truth trees that correspond to specific sequent proofs

// Simple contradiction: B, ¬B closes
TEST_CASE("Conversion: B and negB close", "[Conversion]")
{
    TreeSerializer ser{};

    UnaryTreeNode B{LogicalAtom{"B"}};
    B.isPremise = true;

    auto negB = std::make_unique<UnaryTreeNode>(LogicalNot{LogicalAtom{"B"}});
    negB->isPremise = true;

    auto closed = std::make_unique<ClosedTreeNode>();
    closed->decomposition1 = B.GetId();
    closed->decomposition2 = negB->GetId();

    negB->SetChild(std::move(closed));
    B.SetChild(std::move(negB));

    std::string json = ser.Serialize(B);

    REQUIRE(ser.serializedNodes.size() == 3);
    REQUIRE(json.find("\"text\":\"B\"") != std::string::npos);
    REQUIRE(json.find("\"text\":\"¬B\"") != std::string::npos);
    REQUIRE(json.find("\"text\":\"×\"") != std::string::npos);
}

// Disjunction branches: A ∨ B → left A, right B (both open)
TEST_CASE("Conversion: disjunction branches", "[Conversion]")
{
    TreeSerializer ser{};

    BinaryTreeNode root{LogicalOr<LogicalAtom>{LogicalAtom{"A"}, LogicalAtom{"B"}}};
    root.isPremise = true;

    auto leftA = std::make_unique<UnaryTreeNode>(LogicalAtom{"A"});
    leftA->antecedent = root.GetId();
    leftA->SetChild(std::make_unique<OpenTreeNode>());

    auto rightB = std::make_unique<UnaryTreeNode>(LogicalAtom{"B"});
    rightB->antecedent = root.GetId();
    rightB->SetChild(std::make_unique<OpenTreeNode>());

    root.SetLeftChild(std::move(leftA));
    root.SetRightChild(std::move(rightB));

    std::string json = ser.Serialize(root);
    REQUIRE(ser.serializedNodes.size() == 5);  // root, A, ◯, B, ◯
}

// Conjunction linear: A ∧ B → A then B on same branch
TEST_CASE("Conversion: conjunction linear decomposition", "[Conversion]")
{
    TreeSerializer ser{};

    UnaryTreeNode root{LogicalAnd<LogicalAtom>{LogicalAtom{"A"}, LogicalAtom{"B"}}};
    root.isPremise = true;

    auto nodeA = std::make_unique<UnaryTreeNode>(LogicalAtom{"A"});
    nodeA->antecedent = root.GetId();
    auto nodeB = std::make_unique<UnaryTreeNode>(LogicalAtom{"B"});
    nodeB->antecedent = root.GetId();
    nodeB->SetChild(std::make_unique<OpenTreeNode>());
    nodeA->SetChild(std::move(nodeB));
    root.SetChild(std::move(nodeA));

    std::string json = ser.Serialize(root);
    REQUIRE(ser.serializedNodes.size() == 4);  // root, A, B, ◯
    REQUIRE(json.find("\"text\":\"A\"") != std::string::npos);
    REQUIRE(json.find("\"text\":\"B\"") != std::string::npos);
}

// Full closed proof: A, ¬A ∨ B, ¬B — both branches close
TEST_CASE("Conversion: full closed proof A and negA-or-B implies B", "[Conversion]")
{
    TreeSerializer ser{};

    UnaryTreeNode premA{LogicalAtom{"A"}};
    premA.isPremise = true;

    auto premDisj = std::make_unique<UnaryTreeNode>(
        LogicalOr<>{LogicalNot{LogicalAtom{"A"}}, LogicalAtom{"B"}}
    );
    premDisj->isPremise = true;

    // ¬B is where the tree branches (negated conclusion for refutation)
    auto premNegB = std::make_unique<BinaryTreeNode>(LogicalNot{LogicalAtom{"B"}});
    premNegB->isPremise = true;

    // left: ¬A, closes with A
    auto leftNegA = std::make_unique<UnaryTreeNode>(LogicalNot{LogicalAtom{"A"}});
    leftNegA->antecedent = premDisj->GetId();
    auto leftClosed = std::make_unique<ClosedTreeNode>();
    leftClosed->decomposition1 = premA.GetId();
    leftClosed->decomposition2 = leftNegA->GetId();
    leftNegA->SetChild(std::move(leftClosed));

    // right: B, closes with ¬B
    auto rightB = std::make_unique<UnaryTreeNode>(LogicalAtom{"B"});
    rightB->antecedent = premDisj->GetId();
    auto rightClosed = std::make_unique<ClosedTreeNode>();
    rightClosed->decomposition1 = rightB->GetId();
    rightClosed->decomposition2 = premNegB->GetId();
    rightB->SetChild(std::move(rightClosed));

    premNegB->SetLeftChild(std::move(leftNegA));
    premNegB->SetRightChild(std::move(rightB));
    premDisj->SetChild(std::move(premNegB));
    premA.SetChild(std::move(premDisj));

    std::string json = ser.Serialize(premA);

    REQUIRE(ser.serializedNodes.size() == 7);
    // no open branches
    REQUIRE(json.find("\"text\":\"◯\"") == std::string::npos);
    // two closures
    size_t firstX = json.find("\"text\":\"×\"");
    REQUIRE(firstX != std::string::npos);
    REQUIRE(json.find("\"text\":\"×\"", firstX + 1) != std::string::npos);
}

}

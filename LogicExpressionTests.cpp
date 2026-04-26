//
// Expression tests - covers serialize, copy, equality, type checks
//

#include "LogicalAtom.hpp"
#include "LogicalAnd.hpp"
#include "LogicalOr.hpp"
#include "LogicalNot.hpp"
#include "LogicalConditional.hpp"
#include "LogicExpression.hpp"
#include "RecursiveCast.hpp"

#include <catch2/catch_test_macros.hpp>

namespace Logic_Project {

// --- Atoms ---

TEST_CASE("Atom Serialization", "[Atomic][Serialize]")
{
    REQUIRE(LogicalAtom{"A"}.Serialize() == "A");
    REQUIRE(LogicalAtom{"P"}.Serialize() == "P");
    REQUIRE(LogicalAtom{"Prop1"}.Serialize() == "Prop1");
}

TEST_CASE("Atom Copy", "[Atomic][Copy]")
{
    LogicalAtom A{"A"};
    auto copy = A.Copy();

    REQUIRE(copy != nullptr);
    REQUIRE(copy->Serialize() == "A");
    REQUIRE(copy->Equals(A));
    REQUIRE(copy.get() != &A);
}

TEST_CASE("Atom Type and Category", "[Atomic][Type]")
{
    LogicalAtom A{"A"};

    REQUIRE(A.GetType() == ExpressionType::Atomic);
    REQUIRE(A.GetCategory() == ExpressionCategory::LeafExpression);
    REQUIRE(A.Is<LogicalAtom>());
    REQUIRE_FALSE(A.Is<LogicalNot<>>());
}

TEST_CASE("Atom equality is reflexive and symmetric", "[Atomic][Equality]")
{
    LogicalAtom A{"A"};
    LogicalAtom A2{"A"};
    LogicalAtom B{"B"};

    REQUIRE(A.Equals(A));
    REQUIRE(A.Equals(A2));
    REQUIRE(A2.Equals(A));
    REQUIRE_FALSE(A.Equals(B));
}

TEST_CASE("Atom not equal to negation of itself", "[Atomic][Equality]")
{
    LogicalAtom A{"A"};
    LogicalNot negA{LogicalAtom{"A"}};
    REQUIRE_FALSE(A.Equals(negA));
}

// --- Negation ---

TEST_CASE("Not Serialization", "[Not][Serialize]")
{
    LogicalNot negA{LogicalAtom{"A"}};
    REQUIRE(negA.Serialize() == "¬A");
}

TEST_CASE("Double Negation Serialization", "[Not][Serialize]")
{
    LogicalNot<LogicalNot<LogicalAtom>> doubleNeg{LogicalNot{LogicalAtom{"A"}}};
    REQUIRE(doubleNeg.Serialize() == "¬¬A");
}

TEST_CASE("Not Copy and Equality", "[Not][Copy]")
{
    LogicalNot negA{LogicalAtom{"A"}};
    auto copy = negA.Copy();

    REQUIRE(copy != nullptr);
    REQUIRE(copy->Equals(negA));
    REQUIRE(negA.Equals(*copy));
}

TEST_CASE("Not Type and Category", "[Not][Type]")
{
    LogicalNot negA{LogicalAtom{"A"}};
    REQUIRE(negA.GetType() == ExpressionType::Not);
    REQUIRE(negA.GetCategory() == ExpressionCategory::UnaryExpression);
}

TEST_CASE("Different negations are not equal", "[Not][Equality]")
{
    LogicalNot negA{LogicalAtom{"A"}};
    LogicalNot negB{LogicalAtom{"B"}};
    REQUIRE_FALSE(negA.Equals(negB));
}

// --- And ---

TEST_CASE("And Serialization", "[And][Serialize]")
{
    LogicalAnd<LogicalAtom> AandB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    REQUIRE(AandB.Serialize() == "(A ∧ B)");
}

TEST_CASE("And Copy and Equality", "[And][Copy]")
{
    LogicalAnd<LogicalAtom> AandB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    auto copy = AandB.Copy();
    REQUIRE(copy != nullptr);
    REQUIRE(copy->Equals(AandB));
}

TEST_CASE("And Type and Category", "[And][Type]")
{
    LogicalAnd<LogicalAtom> AandB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    REQUIRE(AandB.GetType() == ExpressionType::And);
    REQUIRE(AandB.GetCategory() == ExpressionCategory::BinaryExpression);
}

// structural equality cares about order
TEST_CASE("And operand order matters", "[And][Equality]")
{
    LogicalAnd<LogicalAtom> AB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    LogicalAnd<LogicalAtom> BA{LogicalAtom{"B"}, LogicalAtom{"A"}};
    REQUIRE_FALSE(AB.Equals(BA));
}

TEST_CASE("And not equal to Or with same operands", "[And][Equality]")
{
    LogicalAnd<LogicalAtom> AandB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    LogicalOr<LogicalAtom> AorB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    REQUIRE_FALSE(AandB.Equals(AorB));
}

// --- Or ---

TEST_CASE("Or Serialization", "[Or][Serialize]")
{
    LogicalOr<LogicalAtom> AorB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    REQUIRE(AorB.Serialize() == "(A ∨ B)");
}

TEST_CASE("Or Copy and Equality", "[Or][Copy]")
{
    LogicalOr<LogicalAtom> AorB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    auto copy = AorB.Copy();
    REQUIRE(copy != nullptr);
    REQUIRE(copy->Equals(AorB));
}

// --- Nesting ---
TEST_CASE("And inside Or", "[Nested][Serialize]")
{
    // (A ∧ B) ∨ C
    LogicalOr inner{*LogicalAnd{LogicalAtom{"A"}, LogicalAtom{"B"}}.Generalize() , LogicalAtom{"C"}};
    REQUIRE(inner.Serialize() == "((A ∧ B) ∨ C)");
}

TEST_CASE("Or inside And", "[Nested][Serialize]")
{
    // (A ∧ B) ∨ C
    LogicalAnd inner{LogicalOr{LogicalAtom{"A"}, LogicalAtom{"B"}}, LogicalAtom{"C"}};
    REQUIRE(inner.Serialize() == "((A ∨ B) ∧ C)");
}

TEST_CASE("Negation of conjunction", "[Nested][Serialize]")
{
    LogicalAnd<LogicalAtom> AandB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    LogicalNot neg{*AandB.Generalize()};
    REQUIRE(neg.Serialize() == "¬(A ∧ B)");
}

TEST_CASE("Nested expressions equal when structurally identical", "[Nested][Equality]")
{
    LogicalNot neg1{*LogicalAnd<LogicalAtom>{LogicalAtom{"A"}, LogicalAtom{"B"}}.Generalize()};
    LogicalNot neg2{*LogicalAnd<LogicalAtom>{LogicalAtom{"A"}, LogicalAtom{"B"}}.Generalize()};
    REQUIRE(neg1.Equals(neg2));
}

TEST_CASE("Nested expressions not equal with different atoms", "[Nested][Equality]")
{
    LogicalNot neg1{LogicalAnd<LogicalAtom>{LogicalAtom{"A"}, LogicalAtom{"B"}}};
    LogicalNot neg2{LogicalAnd<LogicalAtom>{LogicalAtom{"A"}, LogicalAtom{"C"}}};
    REQUIRE_FALSE(neg1.Equals(neg2));
}

// --- Generalize ---

TEST_CASE("Not Generalize preserves semantics", "[Not][Generalize]")
{
    LogicalNot<LogicalAtom> negA{LogicalAtom{"A"}};
    auto gen = negA.Generalize();
    REQUIRE(gen != nullptr);
    REQUIRE(gen->Serialize() == "¬A");
    REQUIRE(gen->Equals(negA));
}

TEST_CASE("And Generalize preserves semantics", "[And][Generalize]")
{
    LogicalAnd<LogicalAtom> AandB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    auto gen = AandB.Generalize();
    REQUIRE(gen != nullptr);
    REQUIRE(gen->Serialize() == "(A ∧ B)");
}

// --- Or (additional) ---

TEST_CASE("Or Type and Category", "[Or][Type]")
{
    LogicalOr<LogicalAtom> AorB{LogicalAtom{"A"}, LogicalAtom{"B"}};
    REQUIRE(AorB.GetType() == ExpressionType::Or);
    REQUIRE(AorB.GetCategory() == ExpressionCategory::BinaryExpression);
}

// --- Conditional ---

TEST_CASE("Conditional Serialization", "[Conditional][Serialize]")
{
    LogicalConditional<LogicalAtom> cond{LogicalAtom{"A"}, LogicalAtom{"B"}};
    REQUIRE(cond.Serialize() == "(A → B)");
}

TEST_CASE("Conditional Type and Category", "[Conditional][Type]")
{
    LogicalConditional<LogicalAtom> cond{LogicalAtom{"A"}, LogicalAtom{"B"}};
    REQUIRE(cond.GetType() == ExpressionType::Conditional);
    REQUIRE(cond.GetCategory() == ExpressionCategory::BinaryExpression);
    REQUIRE(cond.Is<LogicalConditional<>>());
}

TEST_CASE("Conditional Equality", "[Conditional][Equality]")
{
    LogicalConditional<LogicalAtom> c1{LogicalAtom{"A"}, LogicalAtom{"B"}};
    LogicalConditional<LogicalAtom> c2{LogicalAtom{"A"}, LogicalAtom{"B"}};
    LogicalConditional<LogicalAtom> c3{LogicalAtom{"B"}, LogicalAtom{"A"}};

    REQUIRE(c1.Equals(c2));
    REQUIRE_FALSE(c1.Equals(c3));
}

TEST_CASE("Conditional Copy", "[Conditional][Copy]")
{
    LogicalConditional<LogicalAtom> cond{LogicalAtom{"P"}, LogicalAtom{"Q"}};
    auto copy = cond.Copy();
    REQUIRE(copy != nullptr);
    REQUIRE(copy->Equals(cond));
    REQUIRE(copy->Serialize() == "(P → Q)");
}

TEST_CASE("Conditional not equal to And with same operands", "[Conditional][Equality]")
{
    LogicalConditional<LogicalAtom> cond{LogicalAtom{"A"}, LogicalAtom{"B"}};
    LogicalAnd<LogicalAtom> conj{LogicalAtom{"A"}, LogicalAtom{"B"}};
    REQUIRE_FALSE(cond.Equals(conj));
}

TEST_CASE("Negation of conditional", "[Nested][Serialize]")
{
    LogicalConditional<LogicalAtom> cond{LogicalAtom{"P"}, LogicalAtom{"Q"}};
    LogicalNot neg{*cond.Generalize()};
    REQUIRE(neg.Serialize() == "¬(P → Q)");
}

}

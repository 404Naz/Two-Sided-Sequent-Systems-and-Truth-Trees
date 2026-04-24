# Two-Sided Sequent Systems and Truth Trees

Final project for Dr. Bram's Intermediate Logic Class at RPI, Spring 2026.

Andrew Nazareth and Andrii Bonenberger

## Overview

Converts between two-sided sequent calculus proofs and truth trees (semantic tableaux) for propositional logic. Output targets Willow's JSON format so you can load trees directly into the tool.

## Building

You need CMake 3.22+ and a C++23 compiler (GCC 13+ or Clang 17+). Catch2 gets pulled in automatically.

```bash
mkdir build && cd build
cmake ..
make
```

Run tests:
```bash
ctest --output-on-failure
```

## How it's structured

There are basically three layers:

**Expressions** — an AST for propositional formulas. `LogicalAtom` for variables, `LogicalNot` for negation, `LogicalAnd`/`LogicalOr`/`LogicalConditional` for binary connectives. They're templated so you can do `LogicalAnd<LogicalAtom>` for a typed version or let it generalize to `LogicalAnd<LogicExpression>`. Every expression can `Serialize()` to a string, `Copy()` itself, check `Equals()`, etc.

**Tree nodes** — the truth tree structure. `UnaryTreeNode` for linear steps (one child), `BinaryTreeNode` for branches (two children), `ClosedTreeNode` (×) when you find a contradiction, `OpenTreeNode` (◯) when a branch has no contradiction. Each node gets a unique ID that the serializer uses.

**Sequent nodes** — `SequentNode` with antecedent/succedent lists and a rule enum for all the sequent rules (¬L/R, ∧L/R, ∨L/R, →L/R, plus weakening/contraction/exchange). This part is scaffolding for now — the actual conversion algorithm isn't wired up yet.

**Serializer** — `TreeSerializer` walks the truth tree via visitor pattern and spits out JSON that Willow can read. Format looks like:

```json
{
  "nodes": [
    {"id": 0, "text": "A", "children": [1], "decomposition": [], "premise": true},
    {"id": 1, "text": "¬A", "children": [2], "decomposition": [], "parent": 0, "premise": true},
    {"id": 2, "text": "×", "children": [], "decomposition": [0, 1], "parent": 1}
  ],
  "options": {
    "requireAtomicContradiction": true,
    "requireAllBranchesTerminated": true,
    "lockedOptions": false
  }
}
```

## Building a tree in code

Quick example — B, ¬B closing to contradiction:

```cpp
UnaryTreeNode B{LogicalAtom{"B"}};
B.isPremise = true;

auto negB = std::make_unique<UnaryTreeNode>(LogicalNot{LogicalAtom{"B"}});
negB->isPremise = true;

auto closed = std::make_unique<ClosedTreeNode>();
closed->decomposition1 = B.GetId();
closed->decomposition2 = negB->GetId();

negB->SetChild(std::move(closed));
B.SetChild(std::move(negB));

TreeSerializer ser{};
std::string json = ser.Serialize(B);
```

For branches (like disjunction), use `BinaryTreeNode` with `SetLeftChild()`/`SetRightChild()`.

## Sequent rule → truth tree mapping

Quick reference for how sequent rules map to tree shapes:

| Rule | Tree shape | Notes |
|---|---|---|
| ∧L | Linear | Both conjuncts go on same branch |
| ∧R | Branch | One branch per conjunct |
| ∨L | Branch | One branch per disjunct |
| ∨R | Linear | Both disjuncts go on same branch |
| ¬L/¬R | Linear | Moves formula to the other side |
| →L | Branch | Branches into ¬A and B |
| →R | Linear | Adds A and ¬B to branch |

## What's done and what's not

Done: expression AST, tree nodes, serializer, visitor infrastructure, sequent node types, some unit tests.

Still needs work: the actual conversion algorithm between sequent proofs and truth trees, a parser for input, and the truth tree rules section of the paper.

## Licensing

Uses code from [open-algebra/Oasis](https://github.com/open-algebra/Oasis) (Apache 2.0).

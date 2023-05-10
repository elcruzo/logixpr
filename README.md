# LogiXpr - Formal Logic Proof Generator

A C++ tool that automatically generates the shortest possible proofs for Boolean expression equivalences using formal logic laws.

## Features

- **Expression Parser**: Robust parser with abstract syntax tree (AST) representation
- **Equivalence Engine**: Applies 20+ formal logic laws (De Morgan's, distributive, absorption, etc.)
- **BFS Proof Search**: Breadth-first search with pruning guarantees shortest proofs
- **Interactive Mode**: Command-line interface for exploring logic transformations
- **Optimized Algorithm**: Explores thousands of transformation sequences efficiently

## Supported Logic Operators

| Symbol | Operator | Alternative Symbols |
|--------|----------|-------------------|
| `!`, `~`, `¬` | NOT | |
| `&`, `&&`, `∧` | AND | |
| `\|`, `\|\|`, `∨` | OR | |
| `->`, `→` | IMPLIES | |
| `<->`, `↔` | BICONDITIONAL | |
| `T` | TRUE | |
| `F` | FALSE | |

## Building

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

### Interactive Mode
```bash
./logixpr -i
```

### Prove Equivalence
```bash
./logixpr -p "!(A & B)" "!A | !B"
```

### Generate Equivalent Forms
```bash
./logixpr -g "A -> B"
```

## Example

```
logixpr> prove !(A & B) !A | !B
Searching for proof from:
  ¬(A ∧ B)
to:
  (¬A ∨ ¬B)

Proof found in 1 steps:

 1. (¬A ∨ ¬B)  [De Morgan's Law (AND)]

--- Proof Statistics ---
Proof found: Yes
Total steps: 1
Final expression: (¬A ∨ ¬B)
------------------------
```

## Architecture

- **AST Module** (`ast.h/cpp`): Expression tree representation
- **Parser Module** (`parser.h/cpp`): Tokenization and parsing logic
- **Equivalence Engine** (`equivalence_engine.h/cpp`): Logic law applications
- **Proof Search** (`proof_search.h/cpp`): BFS algorithm with optimizations
- **Logic Laws** (`logic_laws.h`): Formal logic transformation rules

## Logic Laws Implemented

- Double Negation
- De Morgan's Laws
- Distributive Laws
- Absorption Laws
- Identity Laws
- Annihilation Laws
- Complement Laws
- Idempotent Laws
- Commutative Laws
- Associative Laws
- Implication Elimination
- Biconditional Elimination
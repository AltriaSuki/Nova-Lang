# Nova Programming Language

A modern systems programming language with memory safety and zero-cost abstractions.

## Quick Start
```bash
# Build
./scripts/build.sh

# Run compiler
./build/tools/nova/nova examples/hello_world.nova

# Run REPL
./build/tools/nova-repl/nova-repl

# Run tests
./scripts/test.sh
```

## Project Structure
```
lib/Basic/    - Core infrastructure (SourceManager, Diagnostics)
lib/Lex/      - Lexical analysis (Lexer, Token)
lib/Parse/    - Parsing (Parser, AST construction)
lib/AST/      - Abstract syntax tree nodes
lib/Sema/     - Semantic analysis (type checking, borrow checking)
lib/CodeGen/  - Code generation
lib/Driver/   - Compiler driver
tools/        - User-facing executables (nova, nova-repl)
```

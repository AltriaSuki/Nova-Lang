# Nova Architecture

This document describes the **intended compiler architecture** and how code is organized in this repository.

**Status:** Informative. The architecture is implemented incrementally; many `.cpp` files remain placeholders. This document describes the intended decomposition and data flow.

## Pipeline Overview

```
Source (.nova)
  │
  ▼
SourceManager  ──► (text + locations)
  │
  ▼
Lexer          ──► Token stream
  │
  ▼
Parser         ──► AST
  │
  ▼
Sema           ──► Typed AST + symbols
  │
  ▼
Analysis       ──► CFG / ownership / borrow checking (optional)
  │
  ▼
IR             ──► Nova IR (SSA-style)
  │
  ▼
CodeGen        ──► Interpreter or LLVM backend
  │
  ▼
Output         ──► REPL output / object file / executable
```

## Code Layout

- Public headers: `include/nova/`
- Implementations: `lib/`
- Tools/binaries: `tools/`
- Tests: `tests/`

Each compiler subsystem has a matching include/lib directory pair:

- `include/nova/Basic/`, `lib/Basic/` — source locations, file management, identifiers, diagnostics
- `include/nova/Lex/`, `lib/Lex/` — tokens and lexical analysis
- `include/nova/Parse/`, `lib/Parse/` — parsing entry points
- `include/nova/AST/`, `lib/AST/` — AST nodes and ownership
- `include/nova/Sema/`, `lib/Sema/` — symbol tables and type checking
- `include/nova/IR/`, `lib/IR/` — intermediate representation
- `include/nova/Transforms/`, `lib/Transforms/` — IR transforms/optimizations
- `include/nova/CodeGen/`, `lib/CodeGen/` — lowering to interpreter / LLVM IR
- `include/nova/Interpreter/`, `lib/Interpreter/` — tree-walking interpreter/runtime values
- `include/nova/Driver/`, `lib/Driver/` — compilation driver (CLI orchestration)
- `include/nova/Runtime/`, `lib/Runtime/` — runtime hooks and builtins

## Key Data Types (Current)

### Source locations

- `nova::SourceLocation` and `nova::SourceRange` define a compact, file+offset representation.
- `nova::SourceManager` owns file contents and maps offsets to `(line, column)` for diagnostics.

### Tokens

- `nova::TokenKind` defines the token vocabulary (keywords/operators/punctuation/literals).
- `nova::Token` carries kind + `SourceLocation` + length, and optionally `IdentifierInfo*`.
- `nova::IdentifierTable` is intended to intern identifiers and map keywords to token kinds.

## Diagnostics Strategy (Intended)

Diagnostics should be:
- location-aware (primary span + optional ranges)
- categorized (error/warning/note)
- emitted through a single `DiagnosticEngine` so tools/tests can capture messages

The `DiagnosticEngine` API exists but its `.cpp` is currently a placeholder.

## Extending the Compiler

Recommended incremental approach:

1. Implement lexer + token printing (drives `SourceManager`, `IdentifierTable`, `TokenKinds`).
2. Implement a minimal parser for Nova Core (see `docs/language-spec.md`).
3. Add a symbol table + type checker for the same core.
4. Define a small IR and lower the core to IR.
5. Add an interpreter or LLVM lowering for that IR.

Each step should remain testable using unit tests and golden text or IR dumps.

## Suggested Milestones (Concrete)

### M0: Token dump tool

Objective: `nova --dump-tokens examples/hello_world.nova` prints a stable token listing.

Subsystems involved:
- `Basic`: `SourceManager`, `SourceLocation`
- `Lex`: `Lexer`, `Token`, `TokenKinds`
- `Driver`: CLI argument parsing and orchestration

What to test:
- `TokenKinds` spelling/name coverage
- identifiers vs keywords
- numeric/string literal lexing
- locations map to correct `line:column`

### M1: Parser builds AST for expressions

Objective: `nova --dump-ast` prints an AST for Nova Core expressions and `func` items.

Subsystems involved:
- `Parse` + `AST`

What to test:
- precedence/associativity table matches `docs/language-spec.md`
- block expression values (semicolon behavior)

### M2: Type checking

Objective: `nova --type-check` validates Nova Core (no implicit conversions, exact argument matching).

Subsystems involved:
- `Sema`

What to test:
- error messages include primary span and helpful notes

### M3: IR + interpreter/LLVM

Objective: `nova --emit-ir` (and later `--run` / `--emit-llvm`) executes minimal programs.

Subsystems involved:
- `IR`, `Transforms`, `CodeGen`, optionally LLVM backend

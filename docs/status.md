# Project Status (Current Implementation)

This document answers a single question: **what is implemented today**, as distinct from what exists only as scaffolding or planned work.

Nova-Lang is intentionally built as a set of libraries. Many headers and CMake targets exist before the implementation is complete so the project can grow in small vertical slices.

---

## Build Status

- The project configures and builds with CMake.
- Output locations:
  - executables: `build/bin/`
  - libraries: `build/lib/`
- Common developer binaries:
  - unit tests: `build/bin/novaTests`
  - micro-benchmark: `build/bin/nova-bench`

Notes:
- The LLVM backend is optional. Use `-DNOVA_ENABLE_LLVM=OFF` to force-disable it; otherwise it is built only when LLVM is found by CMake.
- Tests use GoogleTest; in restricted environments, CMake `FetchContent` downloads may fail. Build with `-DNOVA_BUILD_TESTS=OFF` if needed.

---

## Current CLI Status

- `build/bin/nova` and `build/bin/nova-repl` are present, but are currently **placeholders** (they print version text and do not compile or execute Nova code).

---

## Module Status (By Directory)

Legend:
- **Implemented**: meaningful behavior exists and is exercised by tests or used by other code.
- **Partial**: some behavior exists, but core features are missing.
- **Scaffold**: headers/targets exist; `.cpp` is placeholder/empty or incomplete.

### `Basic/`

Files:
- `include/nova/Basic/SourceLocation.hpp`, `lib/Basic/SourceLocation.cpp`
- `include/nova/Basic/SourceManager.hpp`, `lib/Basic/SourceManager.cpp`
- `include/nova/Basic/IdentifierTable.hpp`, `lib/Basic/IdentifierTable.cpp`
- `include/nova/Basic/Diagnostic.hpp`, `include/nova/Basic/DiagnosticEngine.hpp`, `lib/Basic/Diagnostic.cpp`, `lib/Basic/DiagnosticEngine.cpp`

Status:
- **Implemented**: `SourceLocation`, `SourceRange`, `SourceManager` basics (file storage, line/column mapping).
- **Partial/Scaffold**: `IdentifierTable` exists but keyword handling strategy is still evolving.
- **Scaffold**: diagnostics engine is currently a placeholder and not used by other subsystems yet.

### `Lex/`

Files:
- `include/nova/Lex/TokenKinds.hpp`, `lib/Lex/TokenKinds.cpp`
- `include/nova/Lex/Token.hpp`, `lib/Lex/Token.cpp`
- `include/nova/Lex/Lexer.hpp`, `lib/Lex/Lexer.cpp`

Status:
- **Implemented**: token kind tables and spelling helpers.
- **Partial**: `Lexer` implements a usable subset (identifiers/keywords, numbers, strings/chars, punctuation, whitespace/comments) and is exercised by `tests/unit/LexerTest.cpp`.
- **Scaffold**: `Token` is currently header-only; `lib/Lex/Token.cpp` is empty.

### `Parse/`, `AST/`, `Sema/`

Files:
- `include/nova/Parse/Parser.hpp`, `lib/Parse/Parser.cpp`
- `include/nova/AST/*.hpp`, `lib/AST/ASTContext.cpp`
- `include/nova/Sema/*.hpp`, `lib/Sema/Sema.cpp`

Status:
- **Scaffold**: APIs exist; implementations are placeholders.

### `IR/` and `Transforms/`

Files:
- `include/nova/IR/*.hpp`, `lib/IR/*.cpp`
- `include/nova/Transforms/Optimizer.hpp`, `lib/Transforms/*.cpp`

Status:
- **Scaffold**: present as build targets; implementations are placeholders/empty.

See also:
- `docs/ir-spec.md` (draft Nova IR v0)

### `Interpreter/` and `Runtime/`

Files:
- `include/nova/Interpreter/*.hpp`, `lib/Interpreter/*.cpp`
- `include/nova/Runtime/Builtin.hpp`, `lib/Runtime/Builtin.cpp`

Status:
- **Scaffold**: interpreter/runtime are placeholders (targets build, but no real execution engine yet).

### `CodeGen/` (including optional LLVM backend)

Files:
- `include/nova/CodeGen/CodeGenModule.hpp`, `lib/CodeGen/CodeGenModule.cpp`
- `include/nova/CodeGen/LLVM/*.hpp`, `lib/CodeGen/LLVM/*.cpp`

Status:
- **Scaffold**: codegen front-end is placeholder.
- **Scaffold**: LLVM backend sources exist but do not yet implement lowering.

### `Driver/`

Files:
- `include/nova/Driver/Driver.hpp`, `lib/Driver/Driver.cpp`

Status:
- **Scaffold**: driver is placeholder (no real CLI pipeline yet).

### `Analysis/`

Files:
- `include/nova/Analysis/*.hpp`, `lib/Analysis/*.cpp`

Status:
- **Scaffold**: borrow/ownership analysis are placeholders.

---

## What “Nova Core” Means Here

The intended minimal language subset is described in:
- `docs/language-spec.md`

Engineering guidance for implementation:
- `docs/lowering-guide.md`
- `docs/testing-guide.md`

At present, Nova Core is **not** implemented end-to-end. The recommended next step is the implementation of a single vertical slice:

1. Implement lexer for Nova Core tokens.
2. Add `nova --dump-tokens <file>` in the driver tool.
3. Add unit tests for lexing + source locations.

---

## Recommended Next Milestones (Incremental)

These milestones are intended to minimize implementation complexity while preserving incremental validation:

- **M0 Token dump**: `--dump-tokens` with correct spans and deterministic output.
- **M1 Expression parser**: `--dump-ast` for Nova Core expressions/blocks/`if`/`while`.
- **M2 Type checker**: `--type-check` for the Nova Core rules in the spec (no implicit conversions).
- **M3 Codegen**: `--emit-llvm` (or `--emit-ir`) for Nova Core, with required traps for integer division corner cases.

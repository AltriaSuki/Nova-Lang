# Testing Guide (Draft)

This document describes a practical testing strategy for Nova-Lang that supports incremental compiler development.

Goals:
- Detect regressions early using short-running unit tests.
- Ensure compiler outputs are deterministic, thereby enabling “golden” tests.
- Validate end-to-end behavior once the compiler can execute programs.

---

## 1. Test Layers

### 1.1 Unit tests (fast)

Location: `tests/unit/`

Unit tests should cover:
- `SourceLocation` / `SourceManager` correctness (line/column mapping)
- token spelling tables
- lexing edge cases (comments, literals, locations)
- parser precedence and associativity
- type checker rule enforcement

### 1.2 Integration tests (medium)

Location: `tests/integration/` (currently a placeholder)

Integration tests should cover:
- parsing + type checking on multi-function input
- diagnostic formatting (spans and messages)

### 1.3 End-to-end tests (slower)

Location: `tests/e2e/` (currently a placeholder)

End-to-end tests should cover:
- compiling a `.nova` program and running it (interpreter or LLVM backend)
- verifying stdout/stderr and exit codes

### 1.4 Benchmarks (optional)

Location: `tests/benchmarks/`

Benchmarks should cover:
- lexer throughput (tokens/s, MiB/s)
- parser throughput (once parsing exists)
- compilation time for representative programs

Current:
- `build/bin/nova-bench` — lexer micro-benchmark (run in Release mode for meaningful numbers)

---

## 2. Deterministic “Dump” Outputs

To facilitate golden tests, debug outputs should be deterministic:

- `--dump-tokens`
- `--dump-ast`
- `--emit-ir`
- `--emit-llvm`

Determinism guidelines:
- stable ordering (no pointer-address-based ordering)
- stable naming (e.g. `%t0`, `%t1` rather than randomized)
- stable formatting (no extra whitespace differences)

---

## 3. Suggested Test Matrix (Nova Core)

### 3.1 Lexer

- keywords vs identifiers
- integer bases: `0x`, `0b`, `0o`, underscores
- floats with exponent
- strings and escapes
- line and block comments
- correct `line:column` for tokens after tabs and newlines

### 3.2 Parser

- precedence:
  - `a = b + c * d`
  - `a || b && c`
- `if` / `else if` / `else` nesting
- block expression semicolon rules
- error recovery:
  - missing `;`
  - missing `)`

### 3.3 Type checking

- `if` condition must be `bool`
- `if` branches must have same type
- no implicit conversions:
  - `i64 + u64` rejects
  - `i64 + f64` rejects
- assignment:
  - immutable assignment rejects
  - mismatched assignment type rejects

### 3.4 Codegen

- short-circuit:
  - RHS is not evaluated when LHS determines result
- integer division:
  - divisor 0 traps
  - `i64::MIN / -1` traps

---

## 4. Golden Test Pattern (Recommended)

After `nova` provides dump flags, a practical pattern is:

- input: `.nova` file under `tests/cases/<area>/name.nova`
- expected: `.txt` file under `tests/cases/<area>/name.<dump>.txt`

Example (suggested layout; directories may not exist yet):

- `tests/cases/lex/keywords.nova`
- `tests/cases/lex/keywords.tokens.txt`

A unit test may execute the relevant subsystem and compare the generated dump text against the expected file contents.

---

## 5. CI Considerations

In environments where dependencies cannot be downloaded, avoid test setups that require network access during CMake configuration (e.g. GoogleTest via `FetchContent`). Prefer:

- system-installed GTest, or
- `-DNOVA_BUILD_TESTS=OFF` for constrained environments.

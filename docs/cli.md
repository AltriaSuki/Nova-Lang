# CLI Specification (Draft)

This document defines the intended command-line interface for the `nova` tool. It is structured to ensure that:

- outputs are deterministic (supports golden tests)
- flags map cleanly to compiler pipeline stages
- adding new flags does not require a redesign

**Status:** Draft. The current `nova` binary is a placeholder.

---

## 1. Executables

- `nova` — compiler driver
- `nova-repl` — interactive REPL (optional milestone)

Both are built into `build/bin/`.

---

## 2. Global Conventions

### 2.1 Exit codes

- `0` — success
- `1` — compile error (diagnostics emitted)
- `2` — internal compiler error (ICE)

### 2.2 Output streams

- diagnostics → stderr
- dumps (`--dump-*`, `--emit-*`) → stdout unless `-o` is used

### 2.3 Determinism

All dump outputs must be deterministic:
- stable ordering
- stable naming
- no pointer addresses
- no timestamps

---

## 3. Common Flags

### 3.1 Input

- `nova <file.nova>`
- `nova -` reads from stdin (filename reported as `<stdin>`)

### 3.2 Output selection

- `-o <path>` — write primary output to a file

For dump modes, `-o` writes the dump output to the file instead of stdout.

### 3.3 Build mode

- `-O0`, `-O1`, `-O2`, `-O3` — optimization level (defaults to `-O0` in Debug builds)

### 3.4 Language version

- `--edition <n>` — select language edition (default `0`)

---

## 4. Pipeline / Debug Flags

These flags should be added early to make development testable.

### 4.1 Lexing

- `--dump-tokens` — print token stream

Suggested format is defined in `docs/language-spec.md` §13.1.

### 4.2 Parsing

- `--dump-ast` — print AST

Suggested format is defined in `docs/language-spec.md` §13.2.

### 4.3 Type checking

- `--type-check` — stop after type checking; produce no output on success

### 4.4 IR/Codegen

- `--emit-ir` — emit Nova IR (if implemented) in a stable textual form
- `--emit-llvm` — emit LLVM IR (if LLVM backend exists)

### 4.5 Running

- `--run` — run the program (via interpreter or JIT, depending on implementation)

---

## 5. Examples (Desired UX)

```bash
# Lex only
nova --dump-tokens examples/hello_world.nova

# Parse only
nova --dump-ast examples/hello_world.nova

# Type-check only
nova --type-check examples/hello_world.nova

# Emit LLVM IR
nova --emit-llvm examples/hello_world.nova -o hello.ll
```

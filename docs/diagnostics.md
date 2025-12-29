# Diagnostics Specification (Draft)

This document defines how Nova diagnostics should behave and be rendered.

Goals:
- predictable, testable output
- precise source locations and spans
- helpful messages with minimal noise

Non-goals:
- reproducing Rust/Clang formatting exactly

---

## 1. Terminology

- **Diagnostic**: a message emitted by the compiler (error/warning/note).
- **Primary span**: the main `SourceRange` where the issue occurs.
- **Secondary span**: additional ranges that provide context.

---

## 2. Severity

Nova uses at least:
- `error`
- `warning`
- `note`

Rules:
- Any `error` makes compilation fail (exit code 1).
- `warning` does not fail by default.
- `note` attaches additional information to an `error`/`warning`.

Optional flags (future):
- `-Werror` to treat warnings as errors
- `-Wno-<group>` to disable certain warnings

---

## 3. Error Codes

Nova Core defines codes in `docs/language-spec.md` §11.1. This document expands how they should be used.

General rules:
- Codes should remain stable across releases/editions.
- Messages can evolve, but code meaning should not.

Examples (Core):
- `E0001 undefined name`
- `E0002 type mismatch`
- `E0007 expected token`

Reserved codes (for future use) include lexing and control-flow errors such as unterminated comments or `break` outside loop.

---

## 4. Rendering (Recommended)

Recommended text form:

```
error[E0002]: type mismatch
  --> path/file.nova:3:12
   |
 3 | let x: i64 = true;
   |            ^^^^^ expected i64, found bool
```

Requirements:
- `file:line:col` must be correct.
- Caret highlight must cover at least the primary span.

### 4.1 Tabs and columns

Nova Core defines columns in terms of byte offsets (see `docs/language-spec.md` Appendix A.5). A renderer may expand tabs for display, but location math must remain correct and deterministic.

### 4.2 Multi-line spans

Multi-line highlights are allowed, but not required for the MVP. For early milestones:
- show the first line of the span
- show a caret underline limited to that line

---

## 5. Diagnostic Content Guidelines

- Prefer “what + where + expected vs found”.
- For type mismatch: always print both the expected and the found type.
- For parse errors: print the expected token(s) and the actual token kind/spelling.
- Avoid cascading errors: recover at statement boundaries when possible.

---

## 6. Testing Diagnostics

Diagnostics should be testable without depending on platform-specific formatting quirks:

- ensure messages are deterministic
- avoid printing memory addresses
- ensure whitespace is stable

Suggested approach:
- golden tests compare the entire stderr output
- unit tests validate `SourceRange` mapping for specific inputs

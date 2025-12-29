# Nova Documentation

This directory contains design documents and draft specifications for the Nova language and an incremental plan for implementing the Nova compiler.

Documentation conventions are described in `docs/writing-style.md`.

## Where To Start

**For language users (draft):**
- `docs/language-spec.md` — the small, testable core syntax/semantics (draft)
- `docs/features.md` — aspirational feature catalog (many items are not implemented)
- `docs/status.md` — what is implemented today vs planned

**For compiler development:**
- `docs/developer-guide.md` — build, tests, workflow, debugging
- `docs/architecture.md` — module boundaries and data flow
- `docs/implement.md` / `docs/implementation-plan.md` — roadmap and task lists
- `docs/cli.md` — intended `nova` CLI flags and conventions (draft)
- `docs/diagnostics.md` — diagnostic rendering and error code conventions (draft)

**For build-system maintenance:**
- `docs/cmake-guide.md` — CMake notes (LLVM backend, tests, output layout)

## Current Implementation Reality

Nova-Lang is currently a scaffolded compiler project. Some core utilities are implemented (source locations, source manager, token kind tables), while most higher-level subsystems have placeholder APIs and/or empty `.cpp` implementations.

When reading documents:
- Treat `language-spec.md` as the *target contract* (what is intended to be defined precisely).
- Treat `features.md` as an *aspirational design catalogue*; syntax and examples may change.
- Treat `implement.md` as the *work queue* (what to implement next).

## Repository Map (Docs)

- `docs/language-spec.md` — lexical structure, grammar sketch, and core semantics (draft)
- `docs/lowering-guide.md` — engineering notes for lowering Nova Core to IR/LLVM
- `docs/ir-spec.md` — draft Nova IR v0 specification (SSA-friendly)
- `docs/testing-guide.md` — testing strategy and suggested test matrix
- `docs/architecture.md` — compiler pipeline and module responsibilities
- `docs/developer-guide.md` — how to build, run, and contribute
- `docs/api/README.md` — API documentation strategy and current public headers

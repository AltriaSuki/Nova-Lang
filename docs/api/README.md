# Nova API Documentation

Nova-Lang is organized as a set of libraries with public headers under `include/nova/`. This document describes the intended approach to documenting and evolving these APIs.

## Current Public Headers (High-Level)

**Basic**
- `include/nova/Basic/SourceLocation.hpp`
- `include/nova/Basic/SourceManager.hpp`
- `include/nova/Basic/IdentifierTable.hpp`
- `include/nova/Basic/Diagnostic.hpp`
- `include/nova/Basic/DiagnosticEngine.hpp` (API present; implementation is a placeholder today)

**Lex**
- `include/nova/Lex/TokenKinds.hpp`
- `include/nova/Lex/Token.hpp`
- `include/nova/Lex/Lexer.hpp` (implementation exists; still evolving toward the full language spec)

**Other subsystems**

Headers exist for AST/Parse/Sema/IR/CodeGen/Interpreter/etc., but many remain placeholders at present. The architecture and intended boundaries are described in `docs/architecture.md`.

## Documentation Strategy

- Prefer maintaining APIs that are small and testable; expand only when required by the next compiler milestone.
- Treat `include/nova/**` as the stable boundary; `lib/**` may evolve more freely.
- Document invariants in the headers (ownership, nullability, range semantics).

## Generating Reference Docs (Planned)

Doxygen (or an alternative documentation generator) may be introduced once the public APIs stabilize.

Example (not yet wired into the build):

```bash
doxygen Doxyfile
```

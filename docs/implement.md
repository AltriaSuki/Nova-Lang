# Nova Implementation Sequence

Quick reference for implementation. See [implementation-plan.md](implementation-plan.md) for details.

Notes:
- This checklist tracks *implementation progress*, not solely file creation. Many files and targets already exist as scaffolding but may contain placeholder code.
- Prefer implementing the smallest end-to-end vertical slice (e.g., `SourceManager → Lexer → token dump`) before expanding breadth.

---

## Table of Contents

- [Phase 1: Core Infrastructure](#phase-1-core-infrastructure-week-1-2)
- [Phase 2: Lexer](#phase-2-lexer-week-3-4)
- [Phase 3: Parser & AST](#phase-3-parser--ast-week-5-8)
- [Phase 4: Semantic Analysis](#phase-4-semantic-analysis-week-9-12)
- [Phase 5: Interpreter](#phase-5-interpreter-week-13-16)
- [Phase 6: Structs & Enums](#phase-6-structs--enums-week-17-20)
- [Phase 7: Generics](#phase-7-generics-week-21-26)
- [Phase 8: Ownership & Borrowing](#phase-8-ownership--borrowing-week-27-34)
- [Phase 9: Traits](#phase-9-traits-week-35-40)
- [Phase 10: Pattern Matching](#phase-10-pattern-matching-week-41-44)
- [Phase 11: Closures](#phase-11-closures-week-45-48)
- [Phase 12: LLVM Backend](#phase-12-llvm-backend-week-49-56)
- [Phase 13: Module System](#phase-13-module-system-week-57-60)
- [Phase 14: Standard Library](#phase-14-standard-library-week-61-68)
- [Phase 15: Macros](#phase-15-macros-week-69-74)
- [Phase 16: Attributes & Polish](#phase-16-attributes--polish-week-75-78)
- [Tools](#tools)
- [Milestones](#milestones)
- [Summary](#summary)

---

## Phase 1: Core Infrastructure (Week 1-2)

- [ ] **SourceLocation** - `include/nova/Basic/SourceLocation.hpp`, `lib/Basic/SourceLocation.cpp` — Encode file positions
- [ ] **SourceManager** - `include/nova/Basic/SourceManager.hpp`, `lib/Basic/SourceManager.cpp` — Manage source files
- [ ] **IdentifierTable** - `include/nova/Basic/IdentifierTable.hpp`, `lib/Basic/IdentifierTable.cpp` — Intern strings/keywords
- [ ] **Diagnostic** - `include/nova/Basic/Diagnostic.hpp`, `lib/Basic/Diagnostic.cpp` — Error message definitions
- [ ] **DiagnosticEngine** - `include/nova/Basic/DiagnosticEngine.hpp`, `lib/Basic/DiagnosticEngine.cpp` — Error reporting

---

## Phase 2: Lexer (Week 3-4)

- [ ] **TokenKinds** - `include/nova/Lex/TokenKinds.hpp`, `lib/Lex/TokenKinds.cpp` — Define token types
- [ ] **Token** - `include/nova/Lex/Token.hpp`, `lib/Lex/Token.cpp` — Token class
- [ ] **Lexer** - `include/nova/Lex/Lexer.hpp`, `lib/Lex/Lexer.cpp` — Tokenization
- [ ] **LexerTest** - `tests/unit/LexerTest.cpp` — Lexer unit tests

---

## Phase 3: Parser & AST (Week 5-8)

- [ ] **ASTNode** - `include/nova/AST/ASTNode.hpp` — Base node class
- [ ] **Type** - `include/nova/AST/Type.hpp`, `lib/AST/Type.cpp` — Type representations
- [ ] **Decl** - `include/nova/AST/Decl.hpp` — Declarations
- [ ] **Stmt** - `include/nova/AST/Stmt.hpp` — Statements
- [ ] **Expr** - `include/nova/AST/Expr.hpp` — Expressions
- [ ] **ASTContext** - `include/nova/AST/ASTContext.hpp`, `lib/AST/ASTContext.cpp` — AST memory arena
- [ ] **Parser** - `include/nova/Parse/Parser.hpp`, `lib/Parse/Parser.cpp` — Main parser
- [ ] **ParserTest** - `tests/unit/ParserTest.cpp` — Parser unit tests

---

## Phase 4: Semantic Analysis (Week 9-12)

- [ ] **Scope** - `include/nova/Sema/Scope.hpp`, `lib/Sema/Scope.cpp` — Lexical scopes
- [ ] **Symbol** - `include/nova/Sema/Symbol.hpp`, `lib/Sema/Symbol.cpp` — Symbol table
- [ ] **Sema** - `include/nova/Sema/Sema.hpp`, `lib/Sema/Sema.cpp` — Semantic analysis
- [ ] **TypeChecker** - `include/nova/Sema/TypeChecker.hpp`, `lib/Sema/TypeChecker.cpp` — Type checking
- [ ] **TypeInference** - `include/nova/Sema/TypeInference.hpp`, `lib/Sema/TypeInference.cpp` — Type inference
- [ ] **SemaTest** - `tests/unit/SemaTest.cpp` — Sema unit tests

---

## Phase 5: Interpreter (Week 13-16)

- [ ] **Value** - `include/nova/Interpreter/Value.hpp`, `lib/Interpreter/Value.cpp` — Runtime values
- [ ] **Environment** - `include/nova/Interpreter/Environment.hpp`, `lib/Interpreter/Environment.cpp` — Variable storage
- [ ] **Interpreter** - `include/nova/Interpreter/Interpreter.hpp`, `lib/Interpreter/Interpreter.cpp` — Tree-walking interpreter
- [ ] **Builtin** - `include/nova/Runtime/Builtin.hpp`, `lib/Runtime/Builtin.cpp` — Built-in functions

---

## Phase 6: Structs & Enums (Week 17-20)

- [ ] **StructDecl** - `include/nova/AST/Decl.hpp` (extend) — Struct definitions
- [ ] **EnumDecl** - `include/nova/AST/Decl.hpp` (extend) — Enum definitions
- [ ] **ImplDecl** - `include/nova/AST/Decl.hpp` (extend) — Impl blocks
- [ ] **StructValue** - `lib/Interpreter/Value.cpp` (extend) — Runtime struct values

---

## Phase 7: Generics (Week 21-26)

- [ ] **GenericParam** - `include/nova/AST/Type.hpp` (extend) — Generic parameters
- [ ] **GenericResolver** - `include/nova/Sema/GenericResolver.hpp`, `lib/Sema/GenericResolver.cpp` — Type substitution
- [ ] **Monomorphization** - `lib/Sema/Monomorphization.cpp` — Specialize generics

---

## Phase 8: Ownership & Borrowing (Week 27-34)

- [ ] **Ownership** - `include/nova/Analysis/Ownership.hpp`, `lib/Analysis/OwnershipAnalysis.cpp` — Track ownership state
- [ ] **Loan** - `include/nova/Analysis/Loan.hpp` — Borrow tracking
- [ ] **Lifetime** - `include/nova/Analysis/Lifetime.hpp` — Lifetime representation
- [ ] **CFG** - `include/nova/Analysis/CFG.hpp`, `lib/Analysis/CFGBuilder.cpp` — Control flow graph
- [ ] **BorrowChecker** - `include/nova/Analysis/BorrowChecker.hpp`, `lib/Analysis/BorrowChecker.cpp` — Borrow checking
- [ ] **BorrowCheckerTest** - `tests/unit/BorrowCheckerTest.cpp` — Borrow checker tests

---

## Phase 9: Traits (Week 35-40)

- [ ] **TraitDecl** - `include/nova/AST/Decl.hpp` (extend) — Trait definitions
- [ ] **TraitResolver** - `include/nova/Sema/TraitResolver.hpp`, `lib/Sema/TraitResolver.cpp` — Trait resolution
- [ ] **TraitBounds** - `lib/Sema/SemaType.cpp` (extend) — Trait bound checking

---

## Phase 10: Pattern Matching (Week 41-44)

- [ ] **Pattern** - `include/nova/AST/Pattern.hpp` — Pattern AST nodes
- [ ] **MatchExpr** - `include/nova/AST/Expr.hpp` (extend) — Match expression
- [ ] **ParsePattern** - `lib/Parse/ParsePattern.cpp` — Pattern parsing
- [ ] **SemaPattern** - `lib/Sema/SemaPattern.cpp` — Exhaustiveness check
- [ ] **PatternMatcher** - `lib/Interpreter/PatternMatcher.cpp` — Runtime matching

---

## Phase 11: Closures (Week 45-48)

- [ ] **ClosureExpr** - `include/nova/AST/Expr.hpp` (extend) — Closure AST
- [ ] **CaptureAnalysis** - `lib/Analysis/CaptureAnalysis.cpp` — Capture inference
- [ ] **SemaClosure** - `lib/Sema/SemaClosure.cpp` — Closure type checking
- [ ] **Closure** - `lib/Interpreter/Closure.cpp` — Runtime closure

---

## Phase 12: LLVM Backend (Week 49-56)

- [ ] **LLVMCodeGen** - `include/nova/CodeGen/LLVMCodeGen.hpp`, `lib/CodeGen/LLVMCodeGen.cpp` — LLVM IR generation
- [ ] **LLVMTypeGen** - `lib/CodeGen/LLVMTypeGen.cpp` — Type mapping
- [ ] **LLVMExprGen** - `lib/CodeGen/LLVMExprGen.cpp` — Expression codegen
- [ ] **LLVMStmtGen** - `lib/CodeGen/LLVMStmtGen.cpp` — Statement codegen

---

## Phase 13: Module System (Week 57-60)

- [ ] **Module** - `include/nova/AST/Module.hpp` — Module AST
- [ ] **ParseModule** - `lib/Parse/ParseModule.cpp` — Module parsing
- [ ] **SemaModule** - `lib/Sema/SemaModule.cpp` — Visibility checking
- [ ] **ModuleLoader** - `lib/Driver/ModuleLoader.cpp` — Module resolution

---

## Phase 14: Standard Library (Week 61-68)

### Core Types (`stdlib/core/`)
- [ ] **prelude** - `stdlib/core/prelude.nova` — Auto-imports
- [ ] **option** - `stdlib/core/option.nova` — Option\<T\>
- [ ] **result** - `stdlib/core/result.nova` — Result\<T,E\>
- [ ] **iter** - `stdlib/core/iter.nova` — Iterator trait
- [ ] **string** - `stdlib/core/string.nova` — String type
- [ ] **ops** - `stdlib/core/ops.nova` — Operator traits
- [ ] **cmp** - `stdlib/core/cmp.nova` — Comparison traits
- [ ] **clone** - `stdlib/core/clone.nova` — Clone/Copy traits
- [ ] **default** - `stdlib/core/default.nova` — Default trait
- [ ] **fmt** - `stdlib/core/fmt.nova` — Formatting traits
- [ ] **mem** - `stdlib/core/mem.nova` — Memory operations
- [ ] **ptr** - `stdlib/core/ptr.nova` — Pointer types
- [ ] **slice** - `stdlib/core/slice.nova` — Slice type

### Collections (`stdlib/collections/`)
- [ ] **vec** - `stdlib/collections/vec.nova` — Vec\<T\>
- [ ] **hashmap** - `stdlib/collections/hashmap.nova` — HashMap\<K,V\>
- [ ] **hashset** - `stdlib/collections/hashset.nova` — HashSet\<T\>
- [ ] **linkedlist** - `stdlib/collections/linkedlist.nova` — LinkedList\<T\>
- [ ] **deque** - `stdlib/collections/deque.nova` — Deque\<T\>

### I/O (`stdlib/io/`)
- [ ] **io** - `stdlib/io/mod.nova` — I/O traits (Read, Write)
- [ ] **file** - `stdlib/io/file.nova` — File operations

### Concurrency (`stdlib/sync/`, `stdlib/thread/`, `stdlib/async/`)
- [ ] **sync** - `stdlib/sync/mod.nova` — Sync primitives
- [ ] **mutex** - `stdlib/sync/mutex.nova` — Mutex\<T\>
- [ ] **rwlock** - `stdlib/sync/rwlock.nova` — RwLock\<T\>
- [ ] **arc** - `stdlib/sync/arc.nova` — Arc\<T\>
- [ ] **thread** - `stdlib/thread/mod.nova` — Threading
- [ ] **async** - `stdlib/async/mod.nova` — Async runtime

### Errors
- [ ] **error** - `stdlib/error.nova` — Error trait

---

## Phase 15: Macros (Week 69-74)

- [ ] **MacroParser** - `include/nova/Macro/MacroParser.hpp`, `lib/Macro/MacroParser.cpp` — Macro definitions
- [ ] **MacroExpander** - `include/nova/Macro/MacroExpander.hpp`, `lib/Macro/MacroExpander.cpp` — Macro expansion
- [ ] **DeclarativeMacro** - `lib/Macro/DeclarativeMacro.cpp` — macro_rules!

---

## Phase 16: Attributes & Polish (Week 75-78)

- [ ] **Attribute** - `include/nova/AST/Attribute.hpp` — Attribute AST
- [ ] **ParseAttribute** - `lib/Parse/ParseAttribute.cpp` — Attribute parsing
- [ ] **Derive** - `lib/CodeGen/Derive.cpp` — Auto derive traits
- [ ] **ErrorFormat** - `lib/Basic/DiagnosticEngine.cpp` (extend) — Human-readable diagnostic rendering

---

## Tools

- [ ] **nova** - `tools/nova/nova.cpp`, `tools/nova/CMakeLists.txt` — Main compiler
- [ ] **nova-repl** - `tools/nova-repl/repl.cpp`, `tools/nova-repl/CMakeLists.txt` — Interactive REPL

---

## Milestones

- [ ] **M1** Lexer tokenizes Nova syntax (Week 4)
- [ ] **M2** Parser generates AST (Week 8)
- [ ] **M3** Type checker works (Week 12)
- [ ] **M4** Interpreter runs programs (Week 16)
- [ ] **M5** Structs & enums work (Week 20)
- [ ] **M6** Generics work (Week 26)
- [ ] **M7** Borrow checker works (Week 34)
- [ ] **M8** Traits work (Week 40)
- [ ] **M9** Pattern matching works (Week 44)
- [ ] **M10** Closures work (Week 48)
- [ ] **M11** LLVM backend compiles (Week 56)
- [ ] **M12** Multi-file projects (Week 60)
- [ ] **M13** Standard library (Week 68)
- [ ] **M14** Macros work (Week 74)
- [ ] **M15** Production ready (Week 78)

---

## Summary

| Phase | Description | Items |
|-------|-------------|-------|
| 1 | Core Infrastructure | 5 |
| 2 | Lexer | 4 |
| 3 | Parser & AST | 8 |
| 4 | Semantic Analysis | 6 |
| 5 | Interpreter | 4 |
| 6 | Structs & Enums | 4 |
| 7 | Generics | 3 |
| 8 | Ownership & Borrowing | 6 |
| 9 | Traits | 3 |
| 10 | Pattern Matching | 5 |
| 11 | Closures | 4 |
| 12 | LLVM Backend | 4 |
| 13 | Module System | 4 |
| 14 | Standard Library | 29 |
| 15 | Macros | 3 |
| 16 | Attributes & Polish | 4 |
| — | Tools | 2 |
| — | Milestones | 15 |
| **Total** | | **113** |

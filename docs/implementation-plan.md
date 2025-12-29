# Nova Language Implementation Plan

A complete roadmap to build a Rust-like language with ownership, borrowing, generics, and traits.

**Estimated Total Time:** 18-24 months (part-time)

---

## Table of Contents

- [Phase 1: Core Infrastructure & Lexer](#phase-1-core-infrastructure--lexer-weeks-1-6)
- [Phase 2: Parser & AST](#phase-2-parser--ast-weeks-7-14)
- [Phase 3: Semantic Analysis & Type Checking](#phase-3-semantic-analysis--type-checking-weeks-15-22)
- [Phase 4: Basic Interpreter](#phase-4-basic-interpreter-weeks-23-30)
- [Phase 5: Structs & Enums](#phase-5-structs--enums-weeks-31-38)
- [Phase 6: Generics](#phase-6-generics-weeks-39-50)
- [Phase 7: Ownership & Borrowing](#phase-7-ownership--borrowing-weeks-51-66)
- [Phase 8: Traits](#phase-8-traits-weeks-67-78)
- [Phase 9: Pattern Matching](#phase-9-pattern-matching-weeks-79-86)
- [Phase 10: Closures & Functional Features](#phase-10-closures--functional-features-weeks-87-94)
- [Phase 11: LLVM Code Generation](#phase-11-llvm-code-generation-weeks-95-110)
- [Phase 12: Module System](#phase-12-module-system-weeks-111-118)
- [Phase 13: Standard Library](#phase-13-standard-library-weeks-119-130)
- [Phase 14: Macros](#phase-14-macros-weeks-131-142)
- [Phase 15: Attributes & Derives](#phase-15-attributes--derives-weeks-143-148)
- [Phase 16: Error Messages & Polish](#phase-16-error-messages--polish-weeks-149-156)
- [Final Project Structure](#final-project-structure)
- [Summary Timeline](#summary-timeline)

---

## Phase 1: Core Infrastructure & Lexer (Weeks 1-6)

### Goal
Tokenize Nova source code into a stream of tokens.

### Files to Implement

#### Already Implemented
- `include/nova/Basic/SourceLocation.hpp`
- `include/nova/Basic/SourceManager.hpp`
- `include/nova/Basic/IdentifierTable.hpp`
- `include/nova/Lex/TokenKinds.hpp`
- `lib/Basic/SourceLocation.cpp`
- `lib/Basic/SourceManager.cpp`
- `lib/Basic/IdentifierTable.cpp`
- `lib/Lex/TokenKinds.cpp`

#### To Implement

| File | Tasks |
|------|-------|
| `include/nova/Basic/Diagnostic.hpp` | [ ] Define DiagnosticID enum, DiagnosticSeverity |
| `lib/Basic/Diagnostic.cpp` | [ ] Implement get_diagnostic_format(), get_default_severity() |
| `include/nova/Basic/DiagnosticEngine.hpp` | [ ] DiagnosticBuilder, DiagnosticEngine classes |
| `lib/Basic/DiagnosticEngine.cpp` | [ ] Implement report(), emit(), format_diagnostic() |
| `include/nova/Lex/Token.hpp` | [ ] Token class with kind, location, spelling |
| `lib/Lex/Token.cpp` | [ ] Token methods: getString(), is(), isOneOf() |
| `include/nova/Lex/Lexer.hpp` | [ ] Lexer class declaration |
| `lib/Lex/Lexer.cpp` | [ ] lex(), skip_whitespace_and_comments() |
| | [ ] lex_identifier() - keywords + identifiers |
| | [ ] lex_number() - int, float, hex, binary, octal |
| | [ ] lex_string() - string literals with escapes |
| | [ ] lex_char() - character literals |
| | [ ] All operators and punctuation |

### Tests
| File | Tests |
|------|-------|
| `tests/unit/LexerTest.cpp` | [ ] Keywords tokenization |
| | [ ] Identifier tokenization |
| | [ ] Number literals (all formats) |
| | [ ] String literals with escapes |
| | [ ] All operators |
| | [ ] Comments (line and block) |
| | [ ] Error recovery |

### Milestone
```nova
// This should tokenize correctly:
func main() {
    let x: i32 = 42
    let y = 3.14
    let s = "hello\nworld"
    // comment
    /* block comment */
}
```

---

## Phase 2: Parser & AST (Weeks 7-14)

### Goal
Parse tokens into an Abstract Syntax Tree.

### Files to Create

#### AST Node Headers (`include/nova/AST/`)

| File | Contents |
|------|----------|
| `ASTNode.hpp` | [ ] Base ASTNode class, NodeKind enum |
| `Type.hpp` | [ ] Type, BuiltinType, PointerType, ReferenceType, ArrayType, TupleType |
| `Decl.hpp` | [ ] Decl base, VarDecl, FuncDecl, ParamDecl, StructDecl, EnumDecl |
| `Stmt.hpp` | [ ] Stmt base, CompoundStmt, IfStmt, WhileStmt, ForStmt, ReturnStmt, ExprStmt |
| `Expr.hpp` | [ ] Expr base, IntegerLiteral, FloatLiteral, StringLiteral, BoolLiteral |
| | [ ] IdentifierExpr, BinaryExpr, UnaryExpr, CallExpr, IndexExpr |
| | [ ] MemberExpr, CastExpr, AssignExpr |
| `Pattern.hpp` | [ ] Pattern base (for later pattern matching) |

#### AST Implementation (`lib/AST/`)

| File | Tasks |
|------|-------|
| `ASTContext.cpp` | [ ] Memory arena for AST nodes |
| `Type.cpp` | [ ] Type comparison, printing |
| `ASTDumper.cpp` | [ ] Debug printer for AST |

#### Parser (`include/nova/Parse/`, `lib/Parse/`)

| File | Tasks |
|------|-------|
| `include/nova/Parse/Parser.hpp` | [ ] Parser class with token stream |
| `lib/Parse/Parser.cpp` | [ ] Infrastructure: advance(), expect(), match() |
| `lib/Parse/ParseDecl.cpp` | [ ] parse_function_decl() |
| | [ ] parse_var_decl() |
| | [ ] parse_struct_decl() |
| | [ ] parse_enum_decl() |
| `lib/Parse/ParseStmt.cpp` | [ ] parse_compound_stmt() |
| | [ ] parse_if_stmt() |
| | [ ] parse_while_stmt() |
| | [ ] parse_for_stmt() |
| | [ ] parse_return_stmt() |
| `lib/Parse/ParseExpr.cpp` | [ ] parse_expression() - Pratt parser |
| | [ ] parse_primary() |
| | [ ] parse_binary() with precedence |
| | [ ] parse_unary() |
| | [ ] parse_call() |
| | [ ] parse_index() |
| `lib/Parse/ParseType.cpp` | [ ] parse_type() |
| | [ ] parse_reference_type() |
| | [ ] parse_array_type() |

### Operator Precedence Table
```
1  (lowest)  = += -= *= /=        (assignment)
2            ||                    (logical or)
3            &&                    (logical and)
4            == != < <= > >=       (comparison)
5            |                     (bitwise or)
6            ^                     (bitwise xor)
7            &                     (bitwise and)
8            << >>                 (shift)
9            + -                   (additive)
10           * / %                 (multiplicative)
11           ! - & *               (unary prefix)
12 (highest) () [] . ::            (postfix/call/member)
```

### Tests
| File | Tests |
|------|-------|
| `tests/unit/ParserTest.cpp` | [ ] Variable declarations |
| | [ ] Function declarations |
| | [ ] Expression parsing |
| | [ ] Statement parsing |
| | [ ] Operator precedence |
| | [ ] Error recovery |

### Milestone
```nova
func add(a: i32, b: i32) -> i32 {
    return a + b
}

func main() {
    let x = 10
    let y = 20
    let sum = add(x, y)
    if sum > 25 {
        print(sum)
    }
}
```

---

## Phase 3: Semantic Analysis & Type Checking (Weeks 15-22)

### Goal
Type check the AST and build symbol tables.

### Files to Create

#### Semantic Analysis (`include/nova/Sema/`, `lib/Sema/`)

| File | Tasks |
|------|-------|
| `include/nova/Sema/Scope.hpp` | [ ] Scope class (lexical scoping) |
| `include/nova/Sema/Symbol.hpp` | [ ] Symbol, SymbolTable classes |
| `include/nova/Sema/Sema.hpp` | [ ] Sema class declaration |
| `lib/Sema/Scope.cpp` | [ ] Scope enter/exit, lookup |
| `lib/Sema/Symbol.cpp` | [ ] Symbol registration, lookup |
| `lib/Sema/Sema.cpp` | [ ] Main semantic analysis driver |
| `lib/Sema/SemaDecl.cpp` | [ ] check_function_decl() |
| | [ ] check_var_decl() |
| | [ ] check_struct_decl() |
| `lib/Sema/SemaStmt.cpp` | [ ] check_if_stmt() |
| | [ ] check_while_stmt() |
| | [ ] check_return_stmt() |
| `lib/Sema/SemaExpr.cpp` | [ ] check_binary_expr() |
| | [ ] check_call_expr() |
| | [ ] check_assign_expr() |
| `lib/Sema/SemaType.cpp` | [ ] Type compatibility checking |
| | [ ] Implicit conversions |

#### Type System (`include/nova/Sema/`)

| File | Tasks |
|------|-------|
| `TypeChecker.hpp` | [ ] Type unification |
| `TypeInference.hpp` | [ ] Bidirectional type inference |

### Type Checking Rules
```
1. Variable must be declared before use
2. Function arguments must match parameter types
3. Return type must match function declaration
4. Binary operators require compatible types
5. Conditions must be bool
6. Assignment requires compatible types
7. Cannot assign to immutable variable
```

### Tests
| File | Tests |
|------|-------|
| `tests/unit/SemaTest.cpp` | [ ] Type mismatch errors |
| | [ ] Undefined variable errors |
| | [ ] Function call type checking |
| | [ ] Return type checking |
| | [ ] Mutability checking |

### Milestone
```nova
func main() {
    let x: i32 = "hello"  // ERROR: type mismatch
    let y = unknown       // ERROR: undefined variable
    x = 10                // ERROR: cannot assign to immutable
}
```

---

## Phase 4: Basic Interpreter (Weeks 23-30)

### Goal
Execute Nova programs via tree-walking interpretation.

### Files to Create

#### Interpreter (`include/nova/Interpreter/`, `lib/Interpreter/`)

| File | Tasks |
|------|-------|
| `include/nova/Interpreter/Value.hpp` | [ ] Value variant (i32, i64, f32, f64, bool, string, etc.) |
| `include/nova/Interpreter/Environment.hpp` | [ ] Runtime environment (variable storage) |
| `include/nova/Interpreter/Interpreter.hpp` | [ ] Interpreter class |
| `lib/Interpreter/Value.cpp` | [ ] Value operations, type conversions |
| `lib/Interpreter/Environment.cpp` | [ ] Variable get/set, scope management |
| `lib/Interpreter/Interpreter.cpp` | [ ] Main interpreter loop |
| `lib/Interpreter/ExprEvaluator.cpp` | [ ] eval_binary_expr() |
| | [ ] eval_unary_expr() |
| | [ ] eval_call_expr() |
| | [ ] eval_literal() |
| `lib/Interpreter/StmtExecutor.cpp` | [ ] exec_if_stmt() |
| | [ ] exec_while_stmt() |
| | [ ] exec_for_stmt() |
| | [ ] exec_return_stmt() |

#### Built-in Functions (`lib/Runtime/`)

| File | Tasks |
|------|-------|
| `Builtin.cpp` | [ ] print(), println() |
| | [ ] input() |
| | [ ] len() |
| | [ ] type_of() |

### Milestone
```nova
func fibonacci(n: i32) -> i32 {
    if n <= 1 {
        return n
    }
    return fibonacci(n - 1) + fibonacci(n - 2)
}

func main() {
    for i in 0..10 {
        println(fibonacci(i))
    }
}
// Output: 0 1 1 2 3 5 8 13 21 34
```

---

## Phase 5: Structs & Enums (Weeks 31-38)

### Goal
User-defined types with methods.

### Files to Modify/Create

| File | Tasks |
|------|-------|
| `include/nova/AST/Decl.hpp` | [ ] StructDecl fields, FieldDecl |
| | [ ] EnumDecl, EnumVariant |
| | [ ] ImplDecl |
| `lib/Parse/ParseDecl.cpp` | [ ] parse_struct_decl() with fields |
| | [ ] parse_enum_decl() with variants |
| | [ ] parse_impl_block() |
| `lib/Sema/SemaDecl.cpp` | [ ] check_struct_decl() |
| | [ ] check_enum_decl() |
| | [ ] check_impl_block() |
| `include/nova/AST/Expr.hpp` | [ ] StructInitExpr |
| | [ ] EnumVariantExpr |
| `lib/Interpreter/Value.hpp` | [ ] StructValue |
| | [ ] EnumValue |

### Milestone
```nova
struct Point {
    x: i32,
    y: i32
}

impl Point {
    func new(x: i32, y: i32) -> Self {
        Point { x, y }
    }
    
    func distance(&self, other: &Point) -> f64 {
        let dx = (self.x - other.x) as f64
        let dy = (self.y - other.y) as f64
        (dx * dx + dy * dy).sqrt()
    }
}

enum Option<T> {
    Some(T),
    None
}
```

---

## Phase 6: Generics (Weeks 39-50)

### Goal
Parametric polymorphism with monomorphization.

### Files to Create/Modify

| File | Tasks |
|------|-------|
| `include/nova/AST/Type.hpp` | [ ] GenericParam, TypeParam |
| | [ ] GenericType |
| `include/nova/AST/Decl.hpp` | [ ] Add generic params to FuncDecl, StructDecl |
| `lib/Parse/ParseType.cpp` | [ ] parse_generic_params() |
| | [ ] parse_generic_args() |
| `include/nova/Sema/GenericResolver.hpp` | [ ] Type substitution |
| `lib/Sema/GenericResolver.cpp` | [ ] Instantiate generic functions |
| | [ ] Instantiate generic types |
| `lib/Sema/Monomorphization.cpp` | [ ] Generate specialized versions |

### Type Inference for Generics
```
1. Collect constraints from usage
2. Unify type variables
3. Substitute concrete types
4. Generate monomorphized version
```

### Milestone
```nova
func identity<T>(x: T) -> T {
    x
}

struct Pair<T, U> {
    first: T,
    second: U
}

func main() {
    let a = identity(42)        // identity<i32>
    let b = identity("hello")   // identity<str>
    let p = Pair { first: 1, second: "two" }
}
```

---

## Phase 7: Ownership & Borrowing (Weeks 51-66) (High Complexity)

### Goal
Implement Rust-like ownership and borrow checking.

### Files to Create

#### Analysis (`include/nova/Analysis/`, `lib/Analysis/`)

| File | Tasks |
|------|-------|
| `include/nova/Analysis/Ownership.hpp` | [ ] OwnershipState enum (Owned, Moved, Borrowed, MutBorrowed) |
| `include/nova/Analysis/Loan.hpp` | [ ] Loan struct (origin, kind, region) |
| `include/nova/Analysis/Lifetime.hpp` | [ ] Lifetime, Region representation |
| `include/nova/Analysis/CFG.hpp` | [ ] Control Flow Graph |
| `lib/Analysis/CFGBuilder.cpp` | [ ] Build CFG from AST |
| `lib/Analysis/OwnershipAnalysis.cpp` | [ ] Track ownership state per variable |
| | [ ] Detect use-after-move |
| | [ ] Detect double-move |
| `lib/Analysis/BorrowChecker.cpp` | [ ] Track active loans |
| | [ ] Check borrow rules |
| | [ ] Detect conflicting borrows |
| | [ ] Check lifetimes |
| `lib/Analysis/LifetimeInference.cpp` | [ ] Infer lifetimes |
| | [ ] Lifetime constraints |

### Borrow Checking Algorithm
```
1. Build Control Flow Graph (CFG)
2. For each variable, track:
   - Is it initialized?
   - Is it moved?
   - Active borrows (immutable/mutable)
3. At each program point, verify:
   - No use after move
   - No mutable borrow while immutable borrows exist
   - No multiple mutable borrows
   - Borrows do not outlive data
4. Report errors with helpful messages
```

### Key Rules to Implement
```
Rule 1: Each value has exactly one owner
Rule 2: When owner goes out of scope, value is dropped
Rule 3: Can have EITHER:
        - One mutable reference (&mut)
        - Any number of immutable references (&)
Rule 4: References must not outlive referent
```

### Tests
| File | Tests |
|------|-------|
| `tests/unit/BorrowCheckerTest.cpp` | [ ] Use after move |
| | [ ] Double move |
| | [ ] Mutable + immutable conflict |
| | [ ] Multiple mutable borrows |
| | [ ] Dangling reference |
| | [ ] Valid borrow patterns |

### Milestone
```nova
func main() {
    let s1 = String::from("hello")
    let s2 = s1                    // Move
    // println(s1)                 // ERROR: use after move
    
    let mut x = 5
    let r1 = &x
    let r2 = &x                    // OK: multiple immutable
    // let r3 = &mut x             // ERROR: mutable while immutable exists
    println("{} {}", r1, r2)
    
    let r3 = &mut x                // OK: r1, r2 no longer used
    *r3 = 10
}
```

---

## Phase 8: Traits (Weeks 67-78)

### Goal
Implement trait definitions, implementations, and bounds.

### Files to Create/Modify

| File | Tasks |
|------|-------|
| `include/nova/AST/Decl.hpp` | [ ] TraitDecl |
| | [ ] TraitMethod (required vs default) |
| | [ ] ImplTraitDecl |
| `lib/Parse/ParseDecl.cpp` | [ ] parse_trait_decl() |
| | [ ] parse_impl_trait() |
| `include/nova/Sema/TraitResolver.hpp` | [ ] Trait lookup |
| `lib/Sema/TraitResolver.cpp` | [ ] Find trait implementations |
| | [ ] Check trait bounds |
| | [ ] Resolve method calls |
| `lib/Sema/SemaType.cpp` | [ ] Trait bound checking |
| | [ ] Where clause handling |

### Trait System Features
```
1. Trait definition with required methods
2. Default method implementations
3. Trait bounds on generics: <T: Trait>
4. Multiple bounds: <T: Trait1 + Trait2>
5. Where clauses
6. Associated types
7. Blanket implementations
```

### Built-in Traits to Implement
```
- Copy      : Implicit copy on assignment
- Clone     : Explicit .clone() method
- Drop      : Destructor
- Debug     : {:?} formatting
- Display   : {} formatting
- PartialEq : == and !=
- Eq        : Marker for total equality
- PartialOrd: < <= > >=
- Ord       : Total ordering
- Default   : Default::default()
- Iterator  : Iteration protocol
```

### Milestone
```nova
trait Speak {
    func speak(&self) -> String;
    
    func greet(&self) -> String {
        format("Hello, I say: {}", self.speak())
    }
}

struct Dog { name: String }

impl Speak for Dog {
    func speak(&self) -> String {
        String::from("Woof!")
    }
}

func make_noise<T: Speak>(animal: &T) {
    println(animal.speak())
}
```

---

## Phase 9: Pattern Matching (Weeks 79-86)

### Goal
Full pattern matching with match expressions.

### Files to Create/Modify

| File | Tasks |
|------|-------|
| `include/nova/AST/Pattern.hpp` | [ ] WildcardPattern (_) |
| | [ ] IdentifierPattern |
| | [ ] LiteralPattern |
| | [ ] TuplePattern |
| | [ ] StructPattern |
| | [ ] EnumPattern |
| | [ ] RangePattern |
| | [ ] OrPattern (|) |
| `include/nova/AST/Expr.hpp` | [ ] MatchExpr |
| | [ ] MatchArm |
| `lib/Parse/ParseExpr.cpp` | [ ] parse_match_expr() |
| `lib/Parse/ParsePattern.cpp` | [ ] parse_pattern() |
| `lib/Sema/SemaPattern.cpp` | [ ] Exhaustiveness checking |
| | [ ] Pattern type checking |
| | [ ] Guard expression checking |
| `lib/Interpreter/PatternMatcher.cpp` | [ ] Pattern matching runtime |

### Milestone
```nova
enum Result<T, E> {
    Ok(T),
    Err(E)
}

func process(r: Result<i32, String>) {
    match r {
        Result::Ok(n) if n > 0 => println("positive: {}", n),
        Result::Ok(0) => println("zero"),
        Result::Ok(n) => println("negative: {}", n),
        Result::Err(e) => println("error: {}", e)
    }
}

// If-let and while-let
if let Some(x) = optional_value {
    println(x)
}
```

---

## Phase 10: Closures & Functional Features (Weeks 87-94)

### Goal
First-class functions and closures with capture.

### Files to Create/Modify

| File | Tasks |
|------|-------|
| `include/nova/AST/Expr.hpp` | [ ] ClosureExpr |
| | [ ] CaptureKind (ByRef, ByMutRef, ByValue, ByMove) |
| `lib/Parse/ParseExpr.cpp` | [ ] parse_closure() |
| `lib/Sema/SemaClosure.cpp` | [ ] Infer capture modes |
| | [ ] Check closure types |
| `lib/Analysis/CaptureAnalysis.cpp` | [ ] Determine what to capture |
| | [ ] Integrate with borrow checker |
| `lib/Interpreter/Closure.cpp` | [ ] Closure runtime representation |

### Closure Traits
```
Fn      : &self     - can be called multiple times
FnMut   : &mut self - can be called multiple times, may mutate
FnOnce  : self      - consumes captured values, can only be called once
```

### Milestone
```nova
func main() {
    let x = 10
    let add_x = |n| n + x           // Captures x by reference
    
    let mut counter = 0
    let mut increment = || {
        counter += 1                 // Captures counter by mut ref
        counter
    }
    
    let s = String::from("hello")
    let consume = move || {          // Captures s by move
        println(s)
    }
}
```

---

## Phase 11: LLVM Code Generation (Weeks 95-110)

### Goal
Compile to native code via LLVM.

### Files to Create

| File | Tasks |
|------|-------|
| `include/nova/CodeGen/LLVMCodeGen.hpp` | [ ] LLVM code generator class |
| `lib/CodeGen/LLVMCodeGen.cpp` | [ ] Module setup |
| | [ ] Function generation |
| | [ ] Basic block management |
| `lib/CodeGen/LLVMTypeGen.cpp` | [ ] Nova type → LLVM type |
| `lib/CodeGen/LLVMExprGen.cpp` | [ ] Generate expressions |
| `lib/CodeGen/LLVMStmtGen.cpp` | [ ] Generate statements |
| `lib/CodeGen/LLVMDeclGen.cpp` | [ ] Generate declarations |
| `lib/CodeGen/LLVMBuiltin.cpp` | [ ] Built-in function implementations |

### LLVM Integration Steps
```
1. Add LLVM as dependency (CMake find_package)
2. Create LLVM Module, Context
3. Map Nova types to LLVM types
4. Generate LLVM IR for each function
5. Run optimization passes
6. Generate object file
7. Link with system linker
```

### Optimization Passes
```
- mem2reg (promote allocas to registers)
- instcombine (instruction combining)
- simplifycfg (simplify CFG)
- gvn (global value numbering)
- inline (function inlining)
```

### Milestone
```bash
$ nova compile hello.nova -o hello
$ ./hello
Hello, Nova!
```

---

## Phase 12: Module System (Weeks 111-118)

### Goal
Multi-file projects with imports and visibility.

### Files to Create

| File | Tasks |
|------|-------|
| `include/nova/AST/Module.hpp` | [ ] ModuleDecl |
| | [ ] ImportDecl |
| | [ ] ExportDecl |
| `lib/Parse/ParseModule.cpp` | [ ] parse_module_decl() |
| | [ ] parse_use_decl() |
| `lib/Sema/SemaModule.cpp` | [ ] Module resolution |
| | [ ] Visibility checking (pub/priv) |
| | [ ] Circular dependency detection |
| `lib/Driver/ModuleLoader.cpp` | [ ] Find and load modules |
| | [ ] Module caching |

### Module System Rules
```
1. One file = one module (implicit)
2. mod keyword for submodules
3. use for imports
4. pub for public visibility
5. priv (default) for private
6. pub(crate) for crate-internal
```

### Milestone
```nova
// lib.nova
pub mod math
pub mod string

// math.nova
pub func add(a: i32, b: i32) -> i32 { a + b }

// main.nova
use math::add

func main() {
    println(add(1, 2))
}
```

---

## Phase 13: Standard Library (Weeks 119-130)

### Goal
Essential types and functions.

### Files to Create (`stdlib/`)

#### Core Module (`stdlib/core/`)
| File | Tasks |
|------|-------|
| `prelude.nova` | [ ] Auto-imported types and traits |
| `option.nova` | [ ] Option<T> with methods |
| `result.nova` | [ ] Result<T, E> with methods |
| `iter.nova` | [ ] Iterator trait and adaptors |
| `string.nova` | [ ] String type |
| `ops.nova` | [ ] Operator traits (Add, Sub, Mul, Div, Index, Deref) |
| `cmp.nova` | [ ] Comparison traits (PartialEq, Eq, PartialOrd, Ord) |
| `clone.nova` | [ ] Clone and Copy traits |
| `default.nova` | [ ] Default trait |
| `fmt.nova` | [ ] Formatting traits (Debug, Display) |
| `mem.nova` | [ ] Memory operations (size_of, swap, replace) |
| `ptr.nova` | [ ] Pointer types and operations |
| `slice.nova` | [ ] Slice type and operations |

#### Collections Module (`stdlib/collections/`)
| File | Tasks |
|------|-------|
| `vec.nova` | [ ] Vec<T> dynamic array |
| `hashmap.nova` | [ ] HashMap<K, V> |
| `hashset.nova` | [ ] HashSet<T> |
| `linkedlist.nova` | [ ] LinkedList<T> doubly-linked list |
| `deque.nova` | [ ] Deque<T> double-ended queue |

#### I/O Module (`stdlib/io/`)
| File | Tasks |
|------|-------|
| `mod.nova` | [ ] Read and Write traits, stdin, stdout, stderr |
| `file.nova` | [ ] File type and operations |

#### Synchronization Module (`stdlib/sync/`)
| File | Tasks |
|------|-------|
| `mod.nova` | [ ] Synchronization primitives module |
| `mutex.nova` | [ ] Mutex<T> mutual exclusion |
| `rwlock.nova` | [ ] RwLock<T> reader-writer lock |
| `arc.nova` | [ ] Arc<T> atomic reference counting |

#### Threading Module (`stdlib/thread/`)
| File | Tasks |
|------|-------|
| `mod.nova` | [ ] Thread spawn, join, sleep, current |

#### Async Module (`stdlib/async/`)
| File | Tasks |
|------|-------|
| `mod.nova` | [ ] Future trait and async runtime |

#### Error Module (`stdlib/`)
| File | Tasks |
|------|-------|
| `error.nova` | [ ] Error trait and common error types |

### Key Standard Library Types
```nova
// Option
impl<T> Option<T> {
    func is_some(&self) -> bool
    func is_none(&self) -> bool
    func unwrap(self) -> T
    func unwrap_or(self, default: T) -> T
    func map<U>(self, f: fn(T) -> U) -> Option<U>
    func and_then<U>(self, f: fn(T) -> Option<U>) -> Option<U>
}

// Result
impl<T, E> Result<T, E> {
    func is_ok(&self) -> bool
    func is_err(&self) -> bool
    func unwrap(self) -> T
    func expect(self, msg: str) -> T
    func map<U>(self, f: fn(T) -> U) -> Result<U, E>
    func map_err<F>(self, f: fn(E) -> F) -> Result<T, F>
}

// Vec
impl<T> Vec<T> {
    func new() -> Self
    func push(&mut self, value: T)
    func pop(&mut self) -> Option<T>
    func len(&self) -> usize
    func is_empty(&self) -> bool
    func iter(&self) -> Iter<T>
}
```

---

## Phase 14: Macros (Weeks 131-142)

### Goal
Declarative and procedural macros.

### Files to Create

| File | Tasks |
|------|-------|
| `include/nova/Macro/MacroParser.hpp` | [ ] Macro definition parsing |
| `include/nova/Macro/MacroExpander.hpp` | [ ] Macro expansion |
| `lib/Macro/DeclarativeMacro.cpp` | [ ] macro_rules! style macros |
| `lib/Macro/MacroExpander.cpp` | [ ] Token substitution |
| | [ ] Hygiene |
| | [ ] Recursion limits |

### Macro Types
```
1. Declarative macros (macro_rules!)
   - Pattern matching on tokens
   - Simple substitution
   
2. Procedural macros (later)
   - Custom derive
   - Attribute macros
   - Function-like macros
```

### Milestone
```nova
macro_rules! vec {
    () => { Vec::new() };
    ($($x:expr),*) => {
        {
            let mut temp = Vec::new()
            $(temp.push($x);)*
            temp
        }
    };
}

let v = vec![1, 2, 3, 4, 5]
```

---

## Phase 15: Attributes & Derives (Weeks 143-148)

### Goal
Metadata annotations and automatic implementations.

### Files to Create

| File | Tasks |
|------|-------|
| `include/nova/AST/Attribute.hpp` | [ ] Attribute representation |
| `lib/Parse/ParseAttribute.cpp` | [ ] parse_attribute() |
| `lib/Sema/SemaAttribute.cpp` | [ ] Validate attributes |
| `lib/CodeGen/Derive.cpp` | [ ] Derive implementations |

### Built-in Attributes
```
#[derive(Debug, Clone, PartialEq)]  - Auto-implement traits
#[allow(unused)]                    - Suppress warnings
#[deprecated]                       - Mark as deprecated
#[cfg(target_os = "linux")]         - Conditional compilation
#[test]                             - Mark as test function
#[inline]                           - Inline hint
```

### Milestone
```nova
#[derive(Debug, Clone, PartialEq)]
struct Point {
    x: i32,
    y: i32
}

#[test]
func test_point() {
    let p1 = Point { x: 1, y: 2 }
    let p2 = p1.clone()
    assert_eq(p1, p2)
}
```

---

## Phase 16: Error Messages & Polish (Weeks 149-156)

### Goal
User-friendly compiler experience.

### Tasks

| Category | Tasks |
|----------|-------|
| Error Messages | [ ] Colored output |
| | [ ] Source code snippets |
| | [ ] Underline error location |
| | [ ] Suggestion hints |
| | [ ] Name-suggestion diagnostics |
| Warnings | [ ] Unused variables |
| | [ ] Unused imports |
| | [ ] Unreachable code |
| | [ ] Shadowing |
| CLI | [ ] nova build |
| | [ ] nova run |
| | [ ] nova check |
| | [ ] nova fmt |
| | [ ] nova test |
| | [ ] nova doc |
| LSP | [ ] Language Server Protocol |
| | [ ] Completion |
| | [ ] Hover info |
| | [ ] Go to definition |

### Error Message Format
```
error[E0382]: borrow of moved value: `s`
  --> src/main.nova:5:20
   |
 3 |     let s = String::from("hello")
   |         - move occurs here
 4 |     let s2 = s
   |              - value moved here
 5 |     println("{}", s)
   |                   ^ value borrowed here after move
   |
   = help: consider cloning the value: `s.clone()`
```

---

## Final Project Structure

```
Nova-Lang/
├── include/nova/
│   ├── AST/
│   │   ├── ASTNode.hpp
│   │   ├── Decl.hpp
│   │   ├── Expr.hpp
│   │   ├── Stmt.hpp
│   │   ├── Type.hpp
│   │   ├── Pattern.hpp
│   │   └── Module.hpp
│   ├── Analysis/
│   │   ├── BorrowChecker.hpp
│   │   ├── OwnershipAnalysis.hpp
│   │   ├── Lifetime.hpp
│   │   └── CFG.hpp
│   ├── Basic/
│   │   ├── Diagnostic.hpp
│   │   ├── DiagnosticEngine.hpp
│   │   ├── SourceLocation.hpp
│   │   ├── SourceManager.hpp
│   │   └── IdentifierTable.hpp
│   ├── CodeGen/
│   │   ├── Interpreter/
│   │   │   ├── Interpreter.hpp
│   │   │   ├── Value.hpp
│   │   │   └── Environment.hpp
│   │   └── LLVM/
│   │       └── LLVMCodeGen.hpp
│   ├── Lex/
│   │   ├── Lexer.hpp
│   │   ├── Token.hpp
│   │   └── TokenKinds.hpp
│   ├── Macro/
│   │   ├── MacroParser.hpp
│   │   └── MacroExpander.hpp
│   ├── Parse/
│   │   └── Parser.hpp
│   └── Sema/
│       ├── Sema.hpp
│       ├── Scope.hpp
│       ├── Symbol.hpp
│       ├── TraitResolver.hpp
│       └── GenericResolver.hpp
├── lib/
│   └── (matching .cpp files)
├── stdlib/
│   ├── core/
│   │   ├── prelude.nova
│   │   ├── option.nova
│   │   ├── result.nova
│   │   ├── iter.nova
│   │   ├── string.nova
│   │   ├── ops.nova
│   │   ├── cmp.nova
│   │   ├── clone.nova
│   │   ├── default.nova
│   │   ├── fmt.nova
│   │   ├── mem.nova
│   │   ├── ptr.nova
│   │   └── slice.nova
│   ├── collections/
│   │   ├── vec.nova
│   │   ├── hashmap.nova
│   │   ├── hashset.nova
│   │   ├── linkedlist.nova
│   │   └── deque.nova
│   ├── io/
│   │   ├── mod.nova
│   │   └── file.nova
│   ├── sync/
│   │   ├── mod.nova
│   │   ├── mutex.nova
│   │   ├── rwlock.nova
│   │   └── arc.nova
│   ├── thread/
│   │   └── mod.nova
│   ├── async/
│   │   └── mod.nova
│   └── error.nova
│   └── iter/
├── tools/
│   ├── nova/
│   └── nova-repl/
└── tests/
    ├── unit/
    └── integration/
```

---

## Summary Timeline

| Phase | Weeks | Cumulative | Milestone |
|-------|-------|------------|-----------|
| 1. Lexer | 1-6 | 6 weeks | Tokenization works |
| 2. Parser | 7-14 | 14 weeks | AST generation works |
| 3. Sema | 15-22 | 22 weeks | Type checking works |
| 4. Interpreter | 23-30 | 30 weeks | **Programs run!** |
| 5. Structs/Enums | 31-38 | 38 weeks | User types work |
| 6. Generics | 39-50 | 50 weeks | Generic code works |
| 7. Ownership | 51-66 | 66 weeks | **Borrow checker works!** |
| 8. Traits | 67-78 | 78 weeks | Trait system works |
| 9. Patterns | 79-86 | 86 weeks | Pattern matching works |
| 10. Closures | 87-94 | 94 weeks | Closures work |
| 11. LLVM | 95-110 | 110 weeks | **Native compilation!** |
| 12. Modules | 111-118 | 118 weeks | Multi-file projects |
| 13. Stdlib | 119-130 | 130 weeks | Standard library |
| 14. Macros | 131-142 | 142 weeks | Macros work |
| 15. Attributes | 143-148 | 148 weeks | Derives work |
| 16. Polish | 149-156 | 156 weeks | **Production ready!** |

**Total: ~156 weeks (3 years part-time) or ~78 weeks (1.5 years full-time)**

---

Good luck! 🚀

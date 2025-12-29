# Nova Language Specification (Draft)

This document defines the **intended “Nova Core”**: a small, precise subset that is practical to implement end-to-end (lexer → parser → type checker → IR → interpreter/LLVM).

**Implementation status:** the current repository does not yet implement Nova Core end-to-end. Treat this file as the **target contract** the compiler should grow toward.

**Stability:** Draft. Any behavior not explicitly specified in this document is currently unspecified.

---

## Conventions

- Keywords and terminals are written in code font, e.g. `func`.
- Grammar is written in an EBNF-like style.
- “Must” / “must not” are normative requirements for the language.
- “Unspecified” indicates behavior that is not yet defined and should not be relied on.

---

## 1. Lexical Structure

### 1.1 Source Text

- Source files are UTF-8 text.
- Newlines may be `\n` or `\r\n`. Tools should normalize to `\n` for diagnostics.

### 1.2 Whitespace

Whitespace is any of:
- space `U+0020`
- tab `U+0009`
- newline `\n` (and normalized `\r\n`)

Whitespace separates tokens and is otherwise insignificant except inside string/char literals.

### 1.3 Comments

- Line comment: `//` followed by any characters until end of line.
- Block comment: `/*` … `*/`.

Block comment nesting is currently unspecified. Implementations may treat nested block comments as a lexical error.

### 1.4 Keywords

The following keywords are reserved and must not be used as identifiers:

```
func let mut
if else while for return break continue
match
pub priv mod use as
unsafe
true false
```

Type keywords are reserved:

```
i8 i16 i32 i64 isize
u8 u16 u32 u64 usize
f32 f64
bool str char
```

Notes:
- Nova Core *uses* only a small subset of these types; the rest are reserved for extension.

### 1.5 Identifiers

Nova Core identifiers are ASCII-only:

- Start: `A-Z` / `a-z` / `_`
- Continue: `A-Z` / `a-z` / `0-9` / `_`

Unicode identifiers are currently unspecified.

### 1.6 Tokens: Operators and Punctuation

Nova Core recognizes the following operator/punctuator spellings (see `include/nova/Lex/TokenKinds.hpp`):

```
+  -  *  /  %
=  == !=  < <= > >=
&  &&  |  ||
->  =>  .  ::
,  ;  :
( )  { }  [ ]
```

Other operators (bitwise shifts, etc.) are reserved; their semantics are currently unspecified.

### 1.7 Literals

#### 1.7.1 Integer literals

Grammar (draft):

```
IntLit :=
  DecInt
| "0x" HexDigit+
| "0b" BinDigit+
| "0o" OctDigit+

DecInt := Digit (Digit | "_")*
HexDigit := [0-9a-fA-F] | "_"
BinDigit := [0-1] | "_"
OctDigit := [0-7] | "_"
Digit := [0-9]
```

Rules:
- `_` separators may appear between digits; leading/trailing separators are invalid.
- The type of an integer literal is determined by context (see type inference rules).

#### 1.7.2 Floating-point literals

Grammar (draft):

```
FloatLit :=
  Digit+ "." Digit+ Exp?
| Digit+ Exp

Exp := ("e" | "E") ("+" | "-")? Digit+
```

`_` separators in floats are currently unspecified.

#### 1.7.3 String literals

String literals use double quotes: `"..."`.

Escape sequences (core set):
- `\\` backslash
- `\"` quote
- `\n` newline
- `\r` carriage return
- `\t` tab
- `\0` NUL

Unicode escapes are currently unspecified.

Multiline string literals are currently unspecified.

#### 1.7.4 Character literals

Character literals use single quotes: `'a'`.

The same escape set as string literals is supported.

The exact type and encoding of `char` are currently unspecified.

#### 1.7.5 Boolean literals

`true` and `false`.

---

## 2. Types

### 2.1 Nova Core primitive types

Nova Core uses a small set, chosen to be straightforward to lower to LLVM:

- `i64` — 64-bit signed integer
- `u64` — 64-bit unsigned integer
- `f64` — 64-bit IEEE-754 float
- `bool` — boolean
- `()` — unit type (single value)

Other primitive types are reserved for extension but not required for Nova Core.

### 2.2 Type syntax

```
Type :=
  PrimType
| "()"
| PathType

PrimType := "i64" | "u64" | "f64" | "bool"
PathType := Ident ("::" Ident)*
```

References/pointers/arrays/tuples/generics are extensions (see §10).

### 2.3 Type inference (Nova Core)

Nova Core uses a simple local inference model:

- `let x = Expr;` infers `x`’s type from `Expr` if `Expr` has a known type.
- If `Expr` is an integer literal and no type context exists, the default is `i64`.
- If `Expr` is a float literal and no type context exists, the default is `f64`.

This is intentionally small; full Hindley–Milner inference is out of scope for Nova Core.

---

## 3. Grammar (Nova Core)

### 3.1 Module structure

```
Module := Item*

Item :=
  FuncDecl
| UseDecl

UseDecl := "use" Path ";"
Path := Ident ("::" Ident)*
```

Multi-file modules (`mod`) are reserved for extension.

### 3.2 Functions

```
FuncDecl := "func" Ident "(" Params? ")" ReturnType? Block

Params := Param ("," Param)* (",")?
Param := Ident ":" Type

ReturnType := "->" Type
```

Rules:
- A function without `->` returns `()`.
- Parameters are passed by value in Nova Core.

### 3.3 Statements and blocks

Nova is expression-oriented: a block may evaluate to a value.

```
Block := "{" Stmt* Expr? "}"

Stmt :=
  LetStmt
| ReturnStmt
| WhileStmt
| BreakStmt
| ContinueStmt
| ExprStmt
| Block

LetStmt := "let" "mut"? Ident ( ":" Type )? ( "=" Expr )? ";"
ReturnStmt := "return" Expr? ";"
WhileStmt := "while" Expr Block
BreakStmt := "break" ";"
ContinueStmt := "continue" ";"
ExprStmt := Expr ";"
```

Notes:
- `let x: T;` is allowed but yields an uninitialized binding (this behavior is currently unspecified; many compilers disallow this in early milestones).
- For Nova Core, it is recommended to require `=` initializer in the compiler milestone even if the grammar permits omission.

### 3.4 Expressions

Nova Core expressions:

```
Expr := IfExpr | AssignExpr

IfExpr := "if" Expr Block ("else" (IfExpr | Block))?

AssignExpr := LogicOr ( "=" AssignExpr )?

LogicOr := LogicAnd ( "||" LogicAnd )*
LogicAnd := Equality ( "&&" Equality )*
Equality := Relational ( ("==" | "!=") Relational )*
Relational := Add ( ("<" | "<=" | ">" | ">=") Add )*
Add := Mul ( ("+" | "-") Mul )*
Mul := Unary ( ("*" | "/" | "%") Unary )*
Unary := ("-" | "!") Unary | Primary

Primary :=
  Literal
| Ident
| "(" Expr ")"
| Block
| Call

Call := Ident "(" Args? ")"
Args := Expr ("," Expr)* (",")?

Literal := IntLit | FloatLit | StringLit | CharLit | BoolLit
```

Member access (`.`), path calls (`foo::bar()`), and `match` are extensions.

---

## 4. Operator Precedence and Associativity

From lowest to highest precedence:

| Level | Operators | Associativity |
|------:|-----------|---------------|
| 1 | `=` | right |
| 2 | `||` | left |
| 3 | `&&` | left |
| 4 | `==` `!=` | left |
| 5 | `<` `<=` `>` `>=` | left |
| 6 | `+` `-` | left |
| 7 | `*` `/` `%` | left |
| 8 | unary `-` `!` | right |
| 9 | call `f(...)` | left |

---

## 5. Semantics (Nova Core)

### 5.1 Scopes and shadowing

- A block introduces a new lexical scope.
- `let` introduces a binding in the current scope.
- Shadowing is allowed: an inner scope may declare a new `let x = ...;` even if an outer scope has `x`.

### 5.2 Mutability and assignment

- Bindings are immutable by default.
- `let mut x = ...;` creates a mutable binding.
- Assignment `x = Expr` is only permitted if `x` is mutable and in scope.

### 5.3 Block values and semicolons

- `Expr;` evaluates `Expr` and discards its value (statement).
- A block’s value (if any) is the final expression in the block without a trailing `;`.
- A block with no final expression evaluates to `()`.

Example:

```nova
let x = {
    let a = 10;
    a + 1   // no semicolon => block evaluates to 11
};
```

### 5.4 Evaluation order

Nova Core defines left-to-right evaluation order for:
- operands of binary operators
- function call arguments
- statements in a block

This is a language guarantee (not an optimization detail).

### 5.5 `if` expression

- The condition expression must have type `bool`.
- Both branches of an `if` expression must have the same type (or one branch may be absent, in which case the type is `()`).
- `if` without `else` evaluates to `()` when the condition is false.

### 5.6 `while` loop

- The condition expression must have type `bool`.
- The loop body is a block. Its value (if any) is ignored.
- `break;` exits the nearest loop.
- `continue;` jumps to the next iteration.

Value-producing `break Expr;` is reserved for extension.

### 5.7 `return`

- `return;` returns `()`.
- `return Expr;` returns the value of `Expr`.
- In a function with declared return type `T`, any `return Expr;` must have `Expr: T`.

---

## 6. Static Semantics (Typing Rules)

Nova Core is statically typed.

### 6.0 Notation

Typing judgments are written as:

```
Γ ⊢ e : T
```

Where:
- `Γ` (environment) maps identifiers to their types and mutability.
- `e` is an expression.
- `T` is a type.

Statement judgments are written as:

```
Γ ⊢ s ⇒ Γ'
```

Meaning statement `s` type-checks under `Γ` and produces an updated environment `Γ'` (e.g. after a `let` binding).

### 6.1 Name resolution (Nova Core)

- An identifier expression `x` is well-typed if `x ∈ dom(Γ)`.
- If `x ∉ dom(Γ)`, it is a compile-time error `E0001: undefined name`.

### 6.1 No implicit conversions

Nova Core has no implicit numeric conversions. For example:
- `i64 + u64` is a type error.
- `i64 + f64` is a type error.

Explicit casts are reserved for extension.

### 6.2 Typing of operators

For the following rules, assume operands are already well-typed.

#### 6.2.1 Literals

- Integer literal defaults to `i64` if no expected type exists.
- Float literal defaults to `f64` if no expected type exists.
- `true`/`false` have type `bool`.

Typing (draft):

```
Γ ⊢ n : i64    (when n is an integer literal without expected type)
Γ ⊢ f : f64    (when f is a float literal without expected type)
Γ ⊢ true : bool
Γ ⊢ false : bool
```

**Arithmetic**
- `+ - * / %` are defined for `i64` and `u64` (same-type operands).
- `+ - * /` are defined for `f64` (same-type operands). `%` is not defined for `f64` in Nova Core.

**Comparison**
- `< <= > >=` are defined for numeric types with same-type operands, yielding `bool`.
- `== !=` are defined for `bool` and numeric types with same-type operands, yielding `bool`.
- Equality for strings/chars is reserved for extension.

**Logical**
- `&& ||` require `bool` operands and yield `bool` with short-circuit behavior.
- Unary `!` requires `bool`.

**Unary**
- Unary `-` is defined for `i64` and `f64`. Unary `-` is not defined for `u64` in Nova Core.

#### 6.2.2 Operator typing (more formal)

Let `op` be a binary operator.

Examples:

```
Γ ⊢ e1 : i64   Γ ⊢ e2 : i64
--------------------------------
Γ ⊢ e1 + e2 : i64

Γ ⊢ e1 : T   Γ ⊢ e2 : T   T ∈ { i64, u64, f64 }
-----------------------------------------------
Γ ⊢ e1 < e2 : bool

Γ ⊢ e1 : bool   Γ ⊢ e2 : bool
------------------------------
Γ ⊢ e1 && e2 : bool
```

If the operands are not of the required types, it is a compile-time error `E0002: type mismatch`.

### 6.3 Typing of `let` (statement)

- `let x: T = Expr;` requires `Expr: T`.
- `let x = Expr;` infers `x`’s type from `Expr`.

Formal (draft):

```
Γ ⊢ e : T
----------------------------
Γ ⊢ let x = e; ⇒ Γ[x ↦ (T, imm)]

Γ ⊢ e : T
----------------------------
Γ ⊢ let mut x = e; ⇒ Γ[x ↦ (T, mut)]

Γ ⊢ e : T
----------------------------
Γ ⊢ let x : T = e; ⇒ Γ[x ↦ (T, imm)]
```

If a binding is declared in the same scope with the same name, it is an error `E0003: duplicate binding`. Shadowing is allowed only in an inner scope (see §5.1).

### 6.4 Typing of function calls

- For `f(a1, a2, ...)`, `f` must resolve to a function item in scope.
- Argument count must match parameter count.
- Each argument must match the corresponding parameter type exactly (no implicit conversions).

Name resolution for overloaded functions is reserved for extension.

Formal (draft):

If `Γ(f) = (T1, T2, ..., Tn) -> R` and `Γ ⊢ ai : Ti` for all `i`, then:

```
Γ ⊢ f(a1, ..., an) : R
```

If `f` is not a function, error `E0004: not callable`.
If arity mismatches, error `E0005: wrong argument count`.

### 6.5 Typing of assignment

Assignment is an expression in Nova Core:

- If `x` is mutable and `Γ(x) = (T, mut)` and `Γ ⊢ e : T`, then `Γ ⊢ (x = e) : T`.
- If `x` is immutable, error `E0006: cannot assign to immutable binding`.
- If `x` is not in scope, error `E0001: undefined name`.

### 6.6 Typing of `if`

- Condition must be `bool`.
- If both branches exist, they must have the same type.
- If `else` is absent, the `if` expression has type `()`.

Formal (draft):

```
Γ ⊢ c : bool   Γ ⊢ t : T   Γ ⊢ e : T
-------------------------------------
Γ ⊢ if c t else e : T

Γ ⊢ c : bool   Γ ⊢ t : T
-------------------------
Γ ⊢ if c t : ()
```

### 6.7 Typing of blocks

Blocks introduce a nested environment `Γ_inner`:

- Statements extend the environment inside the block.
- The optional final expression determines the block’s type.

Formal (sketch):

```
Γ ⊢ s1 ⇒ Γ1   Γ1 ⊢ s2 ⇒ Γ2  ...  Γk ⊢ sk ⇒ Γ'
Γ' ⊢ e : T
---------------------------------------------
Γ ⊢ { s1 ... sk e } : T
```

If the block has no trailing expression, its type is `()`.

---

## 7. Runtime Semantics (Draft)

### 7.1 Runtime errors

Nova Core defines runtime error behavior for certain conditions. The default strategy is to **trap** (abort) with a diagnostic. The exact user-facing behavior is currently unspecified.

Examples of runtime errors:
- division by zero for integers
- signed division overflow (`i64::MIN / -1`)

### 7.2 Integer overflow

Overflow behavior must be defined (no undefined behavior). Nova Core defines:

- For `i64` and `u64`, `+ - *` wrap in two’s complement modulo `2^64`.
- For `i64`, unary negation `-x` wraps modulo `2^64` (so `-i64::MIN == i64::MIN`).
- Integer division and remainder trap on:
  - divisor `0`
  - for signed `i64`, `i64::MIN / -1` and `i64::MIN % -1`

**LLVM note:** do not attach `nsw`/`nuw` flags to wrapping operations, and implement division with explicit checks because LLVM `sdiv`/`srem` have undefined behavior on the overflow case.

### 7.3 Floating-point behavior

`f64` operations follow IEEE-754 behavior as implemented by the target platform/LLVM:
- division by zero yields `+∞`/`-∞` as appropriate
- invalid operations may yield `NaN`

Floating-point exception flags are not observable in Nova Core.

---

## 8. Standard Library and Builtins (Reserved)

Nova Core does not require a standard library, but the following are reserved as likely builtins:

- `print(str)` / `println(str)` or similar
- `panic(str)`

The calling convention and string type are currently unspecified.

---

## 9. Examples (Nova Core)

### 9.1 Basic arithmetic

```nova
func main() {
    let x = 40 + 2;
    let y: i64 = x * 2;
    let ok = y == 84;
}
```

### 9.2 `if` as an expression

```nova
func main() {
    let n: i64 = 10;
    let sign = if n < 0 { -1 } else { 1 };
}
```

### 9.3 `while`

```nova
func main() {
    let mut i: i64 = 0;
    while i < 3 {
        i = i + 1;
    }
}
```

---

## 10. Planned Extensions (Non-Core)

These are intentionally excluded from Nova Core but are expected future directions:

- `match`, pattern matching, and `enum` data types
- `struct` and member access (`.`), methods, `impl`
- multi-file modules (`mod`), package system
- generics and traits (static + dynamic dispatch)
- references/pointers, ownership/borrowing, RAII (`Drop`)
- explicit casts, numeric widening/narrowing rules
- error handling via `Option`/`Result` and `?`

---

## 11. Diagnostics and Errors (Normative for Nova Core)

Nova Core requires diagnostics to be:
- **deterministic** (same input → same output)
- **span-based** (each error has a primary `SourceRange` and optional secondary ranges)
- **actionable** (message should name the construct and expected type/token)

### 11.1 Error codes

Nova Core defines the following error codes. Implementations may add more, but should not change the meaning of these codes.

- `E0001 undefined name` — identifier not found in the current environment
- `E0002 type mismatch` — expression type does not match expected type
- `E0003 duplicate binding` — binding redeclared in the same scope
- `E0004 not callable` — attempt to call a non-function
- `E0005 wrong argument count` — call arity mismatch
- `E0006 cannot assign to immutable binding` — assignment to non-`mut` binding
- `E0007 expected token` — parser expected a specific token and did not find it
- `E0008 unexpected token` — parser encountered a token not valid in context
- `E0009 missing return value` — function requires return value but path returns `()`

### 11.2 Message format (recommended)

For human-facing messages, a recommended format is:

```
error[E0002]: type mismatch
  --> file.nova:line:col
   |
 N | <source line>
   |      ^^^^^ expected i64, found bool
```

The exact rendering is not standardized in Nova Core, but line/column reporting must be correct.

---

## 12. Reference Lowering (Engineering Guidance)

This section is non-normative language design guidance intended to make the spec directly implementable with LLVM.

For a first implementation, it is acceptable to lower Nova Core **directly to LLVM IR** without a separate internal IR.

### 12.1 Type lowering to LLVM

Recommended mapping:

| Nova type | LLVM type |
|----------|-----------|
| `i64` | `i64` |
| `u64` | `i64` (use unsigned predicates/ops where relevant) |
| `f64` | `double` |
| `bool` | `i1` |
| `()` | (implementation choice) `void` for function returns; otherwise a dummy `i8` constant |

Notes:
- LLVM has no distinct signed/unsigned integer type. The distinction is in the instruction predicate (e.g. `icmp slt` vs `icmp ult`).

### 12.2 Variables: simplest correct lowering

For early milestones, a simple and correct approach is:

1) Create an `alloca` in the function entry block for each `let` binding.
2) `store` the initializer value.
3) `load` on each use.

This is straightforward and can later be optimized by running LLVM’s `mem2reg` (promote allocas to SSA).

### 12.3 Expressions

**Binary arithmetic**
- Use `add`/`sub`/`mul` without `nsw`/`nuw` flags (because Nova Core wraps).
- For `u64`, use the same instructions; interpretation is unsigned.

**Comparisons**
- `i64`: `icmp slt/sle/sgt/sge`
- `u64`: `icmp ult/ule/ugt/uge`
- equality: `icmp eq/ne` for both
- `f64`: `fcmp olt/ole/ogt/oge/oeq/one` (exact NaN policy is “IEEE default” for now)

**Short-circuit operators**
- Lower `&&` and `||` using control flow (not as bitwise ops):
  - `a && b`: if `a` is false, result false; else evaluate `b`
  - `a || b`: if `a` is true, result true; else evaluate `b`
Use basic blocks and a `phi` node to merge results.

**Assignment**
- `x = e`:
  - evaluate `e`
  - `store` into `x`’s alloca
  - the assignment expression evaluates to the stored value (type of `x`)

### 12.4 `if` expression

Lowering sketch:

1) Emit condition `c` (must be `i1`).
2) Create `then`, `else`, and `merge` blocks.
3) Emit `br i1 c, label %then, label %else`.
4) Emit both branches and end them with `br label %merge`.
5) If the `if` expression has a value type `T != ()`, create a `phi T` in `merge`.

For `if` without `else`, the expression type is `()` and no `phi` is needed.

### 12.5 `while` loop

Lowering sketch:

- Create blocks: `cond`, `body`, `exit`.
- Jump to `cond`, branch to `body` or `exit`.
- In `body`, emit statements and branch back to `cond`.
- `break` branches to `exit`, `continue` branches to `cond`.

### 12.6 Integer division/remainder traps (required by §7.2)

Before emitting `sdiv/srem`:
- Check divisor != 0, otherwise trap.
- For signed `i64`, also check not (`lhs == INT_MIN && rhs == -1`), otherwise trap.

Implementation options:
- call a runtime `panic`/`trap` function
- emit `llvm.trap` intrinsic

Do not rely on LLVM’s `sdiv` semantics without these checks.

---

## 13. Conformance Tests (Engineering Guidance)

Nova Core is easiest to implement when each layer has a deterministic “dump” format:

- `--dump-tokens` (lexer)
- `--dump-ast` (parser)
- `--type-check` (sema)
- `--emit-llvm` or `--emit-ir` (codegen)

These flags are recommended for implementation even if they are not present yet.

### 13.1 Token dump format (suggested)

One token per line:

```
<line>:<col>  <kind>  <spelling>
```

Where:
- `<kind>` is the `TokenKind` name (e.g. `kw_func`, `identifier`, `numeric_constant`)
- `<spelling>` is the exact source slice for identifiers/literals/punctuation

### 13.2 AST dump format (suggested)

Use an indented, stable tree form:

```
FuncDecl name=main
  Block
    Let name=x type=i64
      Binary op=+
        IntLit 40
        IntLit 2
```

### 13.3 Minimum test matrix

Recommended Nova Core test cases:

- Lexing:
  - keywords vs identifiers
  - numeric literal bases and `_` separators
  - comments and whitespace
  - correct `line:col` locations
- Parsing:
  - precedence and associativity (especially `=` vs `+/*`)
  - `if` / `else if` nesting
  - block expression semicolon behavior
- Typing:
  - no implicit conversions
  - `if` branch type mismatch
  - assignment to immutable
  - short-circuit requires `bool`
- Codegen:
  - `&&`/`||` short-circuit (evaluate RHS only when needed)
  - division by zero traps
  - signed division overflow trap

---

## 14. Versioning (Draft)

To maintain language evolvability, the compiler should expose a “language version” concept (e.g. `--edition 0`) and should maintain Nova Core stability within an edition.

---

# Appendix A: Lexing Corner Cases (Normative for Core Tokens)

This appendix specifies corner cases that are frequently a source of ambiguity.

## A.1 Longest-match rule

When multiple token spellings share a prefix, the lexer must choose the **longest valid token**.

Examples:
- `==` must tokenize as `equalequal`, not `equal` then `equal`.
- `<=` must tokenize as `lessequal`, not `less` then `equal`.
- `&&` and `||` must tokenize as `ampamp` / `pipepipe`.
- `->` must tokenize as `arrow`; `=>` as `fatarrow`; `::` as `coloncolon`.

## A.2 `_` separators in integer literals

Rules:
- `_` may appear between digits of an integer literal.
- `_` must not appear:
  - immediately after the base prefix (`0x_1` is invalid)
  - at the start or end (`_1`, `1_` invalid)
  - repeated (`1__2` invalid)

If an invalid separator placement is encountered, the lexer should emit an error diagnostic and produce a best-effort token (implementation strategy is up to the compiler, but output must be deterministic).

## A.3 Ambiguity: `.` vs float literal

Nova Core grammar requires floats in the form `Digit+ "." Digit+` or `Digit+ Exp`.

Therefore:
- `1.` is **not** a valid float literal in Nova Core (tokenizes as `numeric_constant` `1` then `period`).
- `.5` is **not** a valid float literal in Nova Core (tokenizes as `period` then `numeric_constant` `5`).

This avoids ambiguity with future member access and simplifies the lexer.

## A.4 Comments and token boundaries

- `//` starts a comment even if it appears immediately after a token (`x//y` is `identifier` `x` then comment).
- `/*` starts a block comment; contents do not produce tokens.
- Unterminated block comments are a lexing error `E0010 unterminated block comment` (reserved code).

## A.5 Newlines and locations

Source locations are defined in terms of **byte offsets** in the UTF-8 source text.

- Line/column reporting is 1-based.
- Column is defined as “bytes since the last `\n` plus 1” for Nova Core (Unicode column width is currently unspecified).

---

# Appendix B: Control-Flow and Reachability (Draft)

Nova Core has `return`, `break`, and `continue`, which affect reachability.

Guidelines (recommended for early sema):
- A statement after `return;` in the same block is unreachable and should produce a warning (the error/warning policy is currently unspecified).
- `break`/`continue` outside a loop is an error:
  - `E0011 break outside loop`
  - `E0012 continue outside loop`

For Nova Core MVP, it is acceptable to implement only the “outside loop” checks and skip full reachability analysis.

---

# Appendix C: Big-Step Evaluation Semantics (Draft, Formal)

This appendix provides a reference evaluation model for Nova Core. It is intended to make language behavior precise and to guide interpreter/codegen.

Evaluation is defined as:

```
⟨e, σ⟩ ⇓ ⟨v, σ'⟩
```

Where:
- `σ` is a store mapping variables to values.
- `e` is an expression.
- `v` is a value.

Remark: This section is intentionally partial; a production semantics would also model environments and control effects (`return`, loops). The primary objective is to provide an unambiguous account of evaluation order and short-circuiting.

## C.1 Values

Values in Nova Core:
- integers (`i64`/`u64`)
- floats (`f64`)
- booleans
- unit `()`

## C.2 Short-circuit logic

`a && b`:
- Evaluate `a` first.
- If `a` is `false`, the result is `false` and `b` is not evaluated.
- If `a` is `true`, evaluate `b` and the result is `b`.

`a || b`:
- Evaluate `a` first.
- If `a` is `true`, the result is `true` and `b` is not evaluated.
- If `a` is `false`, evaluate `b` and the result is `b`.

These are normative and must be preserved by codegen (see §12.3).

## C.3 `if` expression

Evaluate the condition first:
- If condition is `true`, evaluate the `then` block.
- Otherwise evaluate the `else` block if present; otherwise the result is `()`.

## C.4 Assignment expression

`x = e`:
- Evaluate `e` to `v`.
- Update store `σ[x ↦ v]`.
- Result of the expression is `v`.

---

# Appendix D: Implementation Checklist (Engineering)

This appendix maps Nova Core requirements to concrete compiler work items. It is intended to reduce “spec drift”.

## D.1 Lexer checklist

- Tokenize all operators/punctuation in §1.6 with longest-match (§A.1).
- Track token `SourceLocation` and length; ensure `SourceManager` reports correct `line:col`.
- Implement integer/float literals per §1.7 and the float constraints in §A.3.
- Implement comment skipping per §1.3 and unterminated block comment error (§A.4).

## D.2 Parser checklist

- Implement precedence table in §4.
- Implement semicolon/block-value rules in §5.3.
- Implement `if` nesting (`else if`) per §3.4.
- Provide deterministic parse errors:
  - `E0007 expected token`
  - `E0008 unexpected token`

## D.3 Type checker checklist

- Enforce no implicit conversions (§6.1).
- Enforce operator typing (§6.2).
- Enforce assignment mutability rules (§6.5).
- Enforce `if` condition bool and branch type match (§6.6).

## D.4 Codegen checklist (LLVM)

- Preserve left-to-right evaluation order (§5.4).
- Implement `&&`/`||` short-circuit using CFG+phi (§12.3, Appendix C).
- Implement integer division traps (§7.2, §12.6).
- Do not introduce LLVM UB (Appendix §7.2 notes).

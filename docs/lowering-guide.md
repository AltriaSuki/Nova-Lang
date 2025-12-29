# Lowering Guide: Nova Core → IR/LLVM (Draft)

This document is an engineering companion to `docs/language-spec.md`. It describes a reference lowering strategy that is:

- correct with respect to Nova Core semantics
- sufficiently minimal for an initial compiler implementation
- compatible with LLVM’s semantics (no accidental UB)

---

## 1. General Strategy

Recommended first milestone:

1. Parse Nova Core into an AST.
2. Type-check to assign a type to every expression.
3. Lower directly to LLVM IR (or to Nova IR v0, then LLVM).
4. Optionally run a small set of canonical passes (e.g. mem2reg).

The most direct correct code generation strategy uses **alloca + load/store** for local variables and relies on optimization to recover SSA.

---

## 2. Type Lowering

See `docs/language-spec.md` §12.1 for the suggested mapping.

Key point: **`u64` is still LLVM `i64`**, but comparisons and division must use unsigned predicates/ops.

---

## 3. Expressions

### 3.1 Literals

- `i64/u64`: `ConstantInt`
- `f64`: `ConstantFP`
- `bool`: `i1 0/1`

### 3.2 Identifier lookup

If `x` is a local binding:
- load from `alloca` associated with `x`

### 3.3 Assignment

`x = e`:

1. codegen `e` producing `v`
2. store `v` into `x`’s alloca
3. return `v` as the expression result

### 3.4 Arithmetic

**Wrapping integers**
- use `add/sub/mul` with no `nsw/nuw`

**Floats**
- use `fadd/fsub/fmul/fdiv` (IEEE behavior)

### 3.5 Comparisons

- signed `i64`: `icmp slt/sle/sgt/sge`
- unsigned `u64`: `icmp ult/ule/ugt/uge`
- equality: `icmp eq/ne`
- `f64`: `fcmp` ordered predicates (treat NaN as “not comparable” unless a distinct policy is specified)

### 3.6 Short-circuit `&&` and `||`

Lowering pattern:

- Evaluate LHS condition.
- Branch based on LHS to:
  - a block that produces the final result without evaluating RHS, or
  - a block that evaluates RHS.
- Merge results with a `phi i1`.

This is the key place where “evaluation order is left-to-right” becomes concrete.

### 3.7 `if` expression

If expression has a value type (non-`()`):
- emit both branches and merge with `phi`.

If expression type is `()`:
- it is permissible to omit the `phi` and treat the expression result as “no value”.

### 3.8 Blocks

Blocks are lowered as:
- sequential codegen for statements
- the trailing expression value (if present) becomes the block value

Semicolon behavior:
- expression statements (`Expr;`) discard the computed value

---

## 4. Statements

### 4.1 `let` bindings

For `let mut x: T = e;`:

- create an `alloca T` in the entry block
- codegen initializer `e`
- store into the alloca

Maintain a mapping: `name -> alloca`.

### 4.2 `return`

`return;`:
- if function returns `()`, emit `ret void` (or `ret <unit>` depending on representation)

`return e;`:
- codegen `e` and emit `ret <value>`

### 4.3 `while`, `break`, `continue`

Use classic CFG:

- `cond` block evaluates condition and branches to `body` or `exit`
- `body` ends with branch to `cond`

To support nested loops, maintain a stack of `(breakTarget, continueTarget)` during codegen:

- `break` branches to `breakTarget`
- `continue` branches to `continueTarget`

---

## 5. Avoiding LLVM Undefined Behavior

LLVM is more permissive with undefined behavior than Nova Core. The implementation should ensure that Nova Core’s defined behavior is preserved.

### 5.1 Signed division overflow

Nova Core requires trapping on `i64::MIN / -1` (and `%`).

LLVM `sdiv/srem` on that case is undefined behavior. Emit:

- check divisor != 0
- check not (lhs == INT_MIN && rhs == -1)
- otherwise trap

### 5.2 Division by zero

LLVM integer division by zero is UB. Always emit an explicit check and trap.

### 5.3 Wrapping arithmetic flags

Do not use `nsw`/`nuw` on `add/sub/mul` because Nova Core wraps.

---

## 6. Testing the Lowering

Recommended test styles:

- **golden LLVM IR**: compare `--emit-llvm` output to expected text
- **behavioral tests**: execute minimal programs and compare stdout (later milestone)
- **targeted unit tests**: check that specific constructs produce required checks (e.g. division overflow traps)

See `docs/testing-guide.md` for a concrete test matrix.

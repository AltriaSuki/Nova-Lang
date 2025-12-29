# Nova IR Specification (Draft)

This document specifies a minimal **Nova IR v0** suitable for learning compiler construction. It is intentionally small and designed to map cleanly to LLVM IR.

**Status:** Draft. The repository contains `include/nova/IR/*` and `lib/IR/*` as scaffolding; this document defines the target behavior for a future implementation.

---

## 1. Goals

- Provide a stable, testable mid-level representation between AST and backends.
- Be explicit about control flow (basic blocks) and types.
- Be SSA-friendly (phi nodes), but allow a “lowered” form with temporaries if desired.

Non-goals:
- Full optimization framework
- Complex data layout/aggregate types
- Ownership/borrowing representation

---

## 2. Types

Nova IR v0 types mirror Nova Core:

- `I64` (signed)
- `U64` (unsigned)
- `F64`
- `Bool`
- `Unit`

Backend mapping:
- LLVM `i64` for `I64`/`U64`
- LLVM `double` for `F64`
- LLVM `i1` for `Bool`
- `Unit` is backend-specific (commonly `void` in return position)

---

## 3. Module and Function

### 3.1 Module

A module is a set of functions:

```
Module := Function*
```

### 3.2 Function

```
Function :=
  name: Ident
  params: (Ident: Type)*
  return: Type
  blocks: BasicBlock+
```

Rules:
- Every function has at least one basic block (entry).
- Every basic block ends with a terminator instruction.

---

## 4. Basic Blocks and Control Flow

### 4.1 BasicBlock

```
BasicBlock :=
  label: Ident
  phis: Phi*
  insts: Inst*
  term: Terminator
```

### 4.2 Terminators

- `Ret <value?>`
- `Br <label>`
- `CondBr <cond> <thenLabel> <elseLabel>`
- `Unreachable` (optional)

---

## 5. Instructions (v0)

All instructions define an SSA value unless specified otherwise.

### 5.1 Constants

- `ConstI64 <int>`
- `ConstU64 <int>`
- `ConstF64 <float>`
- `ConstBool true|false`
- `ConstUnit` (optional; can be “no value”)

### 5.2 Integer arithmetic

- `Add <a> <b>`
- `Sub <a> <b>`
- `Mul <a> <b>`

Semantics:
- Wrap modulo `2^64` (for both signed and unsigned), matching Nova Core.

### 5.3 Floating arithmetic

- `FAdd <a> <b>`
- `FSub <a> <b>`
- `FMul <a> <b>`
- `FDiv <a> <b>`

### 5.4 Comparisons

Integers:
- `ICmp <pred> <a> <b>` where `<pred>` is one of `eq ne slt sle sgt sge ult ule ugt uge`

Floats:
- `FCmp <pred> <a> <b>` where `<pred>` is one of `oeq one olt ole ogt oge` (NaN policy is “ordered” by default)

### 5.5 Calls

- `Call <func> (<args>...)`

### 5.6 Phi

- `Phi <type> [<value>, <predBlock>]...`

---

## 6. Undefined Behavior Policy

Nova IR must not have undefined behavior. If an operation could trigger LLVM UB (e.g. signed division overflow), it must be represented explicitly as a trap/check sequence before lowering to LLVM.

---

## 7. Testing Recommendations

Create golden tests for textual IR dumps:

- stable block labels
- stable instruction numbering/naming
- deterministic ordering of phi inputs

Suggested flags:
- `--emit-ir` for Nova IR text
- `--emit-llvm` for LLVM IR text


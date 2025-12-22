## Table of Contents

- [x] [Core Syntax & Basic Types](#core-syntax)
- [ ] [Variables & Mutability](#variables-mutability)
- [ ] [Functions](#functions)
- [ ] [Control Flow](#control-flow)
- [ ] [Ownership & Borrowing](#ownership-borrowing)
- [ ] [Type System & Inference](#type-system-inference)
- [ ] [Generics](#generics)
- [ ] [Pattern Matching](#pattern-matching)
- [ ] [Error Handling](#error-handling)
- [ ] [Object-Oriented Programming](#object-oriented-programming)
- [ ] [Traits & Interfaces](#traits-interfaces)
- [ ] [Functional Programming](#functional-programming)
- [ ] [Module System](#module-system)
- [ ] [Collections](#collections)
- [ ] [Iterators](#iterators)
- [ ] [Concurrency](#concurrency)
- [ ] [Unsafe Code](#unsafe-code)
- [ ] [Macros](#macros)
- [ ] [Attributes](#attributes)
- [ ] [Standard Library](#standard-library)

---

<a id="core-syntax"></a>
### Core Syntax & Basic Types
#### primitive types
```rust
// Integer types (signed and unsigned)
let a: i8 = 127           // 8-bit signed integer
let b: i16 = 32767        // 16-bit signed integer
let c: i32 = 2147483647   // 32-bit signed integer (default)
let d: i64 = 9223372036854775807  // 64-bit signed integer

let e: u8 = 255           // 8-bit unsigned integer
let f: u16 = 65535        // 16-bit unsigned integer
let g: u32 = 4294967295   // 32-bit unsigned integer
let h: u64 = 18446744073709551615 // 64-bit unsigned integer

// Floating-point types
let pi: f32 = 3.14159     // 32-bit float
let e_num: f64 = 2.71828  // 64-bit float (default)

// Boolean
let is_true: bool = true
let is_false: bool = false

// Character (Unicode scalar value)
let letter: char = 'A'
let emoji: char = '😀'

// String slice (immutable view into string data)
let greeting: str = "Hello, Nova!"

// String (owned, growable)
let mut name = String::from("Nova")

// Unit type (represents absence of value)
let unit: () = ()
```

#### type inference
```rust
// Type inference - compiler deduces types
let x = 42              // Inferred as i32
let y = 3.14            // Inferred as f64
let flag = true         // Inferred as bool
let text = "Hello"      // Inferred as str

// Mixed usage
let sum = x + 10        // x is i32, sum is i32
```


#### numeric literals
```rust
// Decimal
let decimal = 98_222    // Underscores for readability

// Hexadecimal
let hex = 0xff

// Octal
let octal = 0o77

// Binary
let binary = 0b1111_0000

// Byte (u8 only)
let byte = b'A'

// Floating point
let float1 = 2.5
let float2 = 1e6        // Scientific notation
let float3 = 3.14_f32   // Explicit type suffix
```
---

<a id="variables-mutability"></a>
### Variables & Mutability

#### immutable by default
```rust
// Immutable variable (default)
let x = 5
// x = 6  // ERROR: cannot assign twice to immutable variable

// Mutable variable (explicit)
let mut y = 5
y = 6      // OK: y is mutable
y += 1     // OK: y is now 7

// Constants (compile-time evaluated)
const MAX_POINTS: i32 = 100_000
const PI: f64 = 3.14159265359
```

#### shadowing
```rust
let x = 5
let x = x + 1        // Shadow previous x
let x = x * 2        // Shadow again (x is now 12)

// Can change type with shadowing
let spaces = "   "
let spaces = spaces.len()  // Now an integer
```

#### destructing
```rust
// Tuple destructuring
let (x, y) = (10, 20)

// Array destructuring
let [first, second, third] = [1, 2, 3]

// Struct destructuring
struct Point { x: i32, y: i32 }
let point = Point { x: 10, y: 20 }
let Point { x, y } = point
```
---

<a id="functions"></a>
### Functions

---

<a id="control-flow"></a>
### Control Flow

---

<a id="ownership-borrowing"></a>
### Ownership & Borrowing

---

<a id="type-system-inference"></a>
### Type System & Inference

---

<a id="generics"></a>
### Generics

---

<a id="pattern-matching"></a>
### Pattern Matching

---

<a id="error-handling"></a>
### Error Handling

---

<a id="object-oriented-programming"></a>
### Object-Oriented Programming

---

<a id="traits-interfaces"></a>
### Traits & Interfaces

---

<a id="functional-programming"></a>
### Functional Programming

---

<a id="module-system"></a>
### Module System

---

<a id="collections"></a>
### Collections

---

<a id="iterators"></a>
### Iterators

---

<a id="concurrency"></a>
### Concurrency

---

<a id="unsafe-code"></a>
### Unsafe Code

---

<a id="macros"></a>
### Macros

---

<a id="attributes"></a>
### Attributes

---

<a id="standard-library"></a>
### Standard Library

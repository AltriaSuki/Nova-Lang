# Nova Language Features

This document presents an aspirational catalogue of language features for Nova.

**Status:** Informative (non-normative). This document describes intended and proposed features; it is not an implementation report.

Most sections use checkboxes (`[ ]`) to indicate planned or not-yet-implemented features, and some examples intentionally use provisional (“future”) syntax that may change as the compiler implementation evolves. For the smallest authoritative contract, prefer `docs/language-spec.md`.

## Design Philosophy

| Principle | Implementation |
|-----------|----------------|
| **Functional-First** | Immutable by default, expressions return values, pattern matching |
| **Strong + Static + Inference** | All types known at compile time, inferred when obvious |
| **RAII** | Scope-based drop — deterministic, predictable resource cleanup |
| **No GC** | Ownership + borrowing, zero runtime overhead |
| **No UB** | Everything defined, no undefined behavior |

```nova
// Functional-first: everything is an expression
let result = if condition { value1 } else { value2 }

let status = match state {
    State::Running => "running",
    State::Stopped => "stopped",
    _ => "unknown"
}

// RAII: scope-based drop
{
    let file = File::open("data.txt")?  // resource acquired
    file.write("hello")
}   // file automatically dropped here, resource released

// Immutable by default
let x = 5           // immutable
let mut y = 10      // explicit mutability
```

## Table of Contents

- [Core Syntax & Basic Types](#core-syntax)
- [Variables & Mutability](#variables-mutability)
- [Operators](#operators)
- [Functions](#functions)
- [Control Flow](#control-flow)
- [Ownership & Borrowing](#ownership-borrowing)
- [Lifetimes](#lifetimes)
- [Type System & Inference](#type-system-inference)
- [Generics](#generics)
- [Pattern Matching](#pattern-matching)
- [Error Handling](#error-handling)
- [Object-Oriented Programming](#object-oriented-programming)
- [Traits & Interfaces](#traits-interfaces)
- [Smart Pointers](#smart-pointers)
- [Functional Programming](#functional-programming)
- [Module System](#module-system)
- [Collections](#collections)
- [Iterators](#iterators)
- [Concurrency](#concurrency)
- [Unsafe Code](#unsafe-code)
- [Macros](#macros)
- [Attributes](#attributes)
- [Testing](#testing)
- [Standard Library](#standard-library)

---

<a id="core-syntax"></a>
## Core Syntax & Basic Types

### [ ] Primitive Types
```nova
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

### [ ] Type Inference
```nova
// Type inference - compiler deduces types
let x = 42              // Inferred as i32
let y = 3.14            // Inferred as f64
let flag = true         // Inferred as bool
let text = "Hello"      // Inferred as str

// Mixed usage
let sum = x + 10        // x is i32, sum is i32
```


### [ ] Numeric Literals
```nova
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

### [ ] Comments
```nova
// Single-line comment

/* 
   Multi-line comment
   can span multiple lines
*/

/// Documentation comment for the following item
/// Supports markdown formatting
/// 
/// # Examples
/// ```
/// let x = 5
/// ```
pub func documented_function() {
    // Implementation
}

//! Module-level documentation comment
//! Describes the current module
```

### [ ] Enums
```nova
// Basic enum
enum Direction {
    North,
    South,
    East,
    West
}

let dir = Direction::North

// Enum with values
enum Color {
    Red = 0xFF0000,
    Green = 0x00FF00,
    Blue = 0x0000FF
}

// Enum with associated data
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32)
}

let msg = Message::Move { x: 10, y: 20 }

// Enum methods
impl Direction {
    func is_vertical(&self) -> bool {
        match self {
            Direction::North | Direction::South => true,
            _ => false
        }
    }
}
```

### [ ] Ranges
```nova
// Exclusive range (end not included)
let range = 0..10       // 0, 1, 2, ..., 9

// Inclusive range (end included)
let inclusive = 0..=10  // 0, 1, 2, ..., 10

// Range in for loop
for i in 0..5 {
    println("{}", i)
}

// Range for slicing
let arr = [0, 1, 2, 3, 4, 5]
let slice = &arr[1..4]   // [1, 2, 3]
let slice = &arr[..3]    // [0, 1, 2] - from start
let slice = &arr[3..]    // [3, 4, 5] - to end
let slice = &arr[..]     // [0, 1, 2, 3, 4, 5] - full

// Range bounds checking
if (0..10).contains(&5) {
    println("5 is in range")
}
```
---

<a id="variables-mutability"></a>
## Variables & Mutability

### [ ] Immutable by Default
```nova
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

### [ ] Shadowing
```nova
let x = 5
let x = x + 1        // Shadow previous x
let x = x * 2        // Shadow again (x is now 12)

// Can change type with shadowing
let spaces = "   "
let spaces = spaces.len()  // Now an integer
```

### [ ] Destructuring
```nova
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

<a id="operators"></a>
## Operators

### [ ] Arithmetic Operators
```nova
let a = 10
let b = 3

let sum = a + b         // Addition: 13
let diff = a - b        // Subtraction: 7
let prod = a * b        // Multiplication: 30
let quot = a / b        // Division: 3 (integer division)
let rem = a % b         // Remainder/Modulo: 1

// Compound assignment
let mut x = 10
x += 5      // x = x + 5 -> 15
x -= 3      // x = x - 3 -> 12
x *= 2      // x = x * 2 -> 24
x /= 4      // x = x / 4 -> 6
x %= 4      // x = x % 4 -> 2

// Floating point division
let f: f64 = 10.0 / 3.0  // 3.333...
```

### [ ] Comparison Operators
```nova
let a = 5
let b = 10

a == b      // Equal: false
a != b      // Not equal: true
a < b       // Less than: true
a <= b      // Less than or equal: true
a > b       // Greater than: false
a >= b      // Greater than or equal: false

// Chained comparisons (if supported)
let x = 5
let valid = 0 < x && x < 10  // true
```

### [ ] Logical Operators
```nova
let t = true
let f = false

t && f      // Logical AND: false
t || f      // Logical OR: true
!t          // Logical NOT: false

// Short-circuit evaluation
let result = false && expensive_function()  // expensive_function not called
let result = true || expensive_function()   // expensive_function not called
```

### [ ] Bitwise Operators
```nova
let a: u8 = 0b1010      // 10
let b: u8 = 0b1100      // 12

a & b       // Bitwise AND: 0b1000 (8)
a | b       // Bitwise OR: 0b1110 (14)
a ^ b       // Bitwise XOR: 0b0110 (6)
!a          // Bitwise NOT: 0b11110101 (245 for u8)

a << 2      // Left shift: 0b101000 (40)
a >> 1      // Right shift: 0b0101 (5)

// Compound assignment
let mut x: u8 = 0b1010
x &= 0b1100     // x = x & 0b1100
x |= 0b0001     // x = x | 0b0001
x ^= 0b1111     // x = x ^ 0b1111
x <<= 1         // x = x << 1
x >>= 2         // x = x >> 2
```

### [ ] Other Operators
```nova
// Reference operators
let x = 5
let r = &x          // Immutable reference
let mr = &mut x     // Mutable reference
let v = *r          // Dereference

// Type cast
let i: i32 = 42
let f: f64 = i as f64

// Range operators (see Ranges section)
0..10       // Exclusive range
0..=10      // Inclusive range

// Member access
struct Point { x: i32, y: i32 }
let p = Point { x: 1, y: 2 }
let x = p.x         // Field access

// Path operator
use std::collections::HashMap  // Module path
let opt = Option::Some(5)      // Enum variant
```

---

<a id="functions"></a>
## Functions

### [ ] Basic Functions
```nova
// Function with explicit return type
func add(a: i32, b: i32) -> i32 {
    return a + b
}

// Implicit return (last expression)
func multiply(a: i32, b: i32) -> i32 {
    a * b  // No semicolon = return value
}

// Function with no return value (returns unit type)
func greet(name: str) {
    println("Hello, {}!", name)
}

// Multiple parameters
func print_point(x: i32, y: i32, z: i32) {
    println("Point({}, {}, {})", x, y, z)
}
```

### [ ] Default Arguments
```nova
func power(base: i32, exponent: i32 = 2) -> i32 {
    let mut result = 1
    for _ in 0..exponent {
        result *= base
    }
    result
}

let squared = power(5)        // Uses default exponent = 2 -> 25
let cubed = power(5, 3)       // Explicit exponent -> 125
```


### [ ] Function Overloading
```nova
// Overload based on parameter count
func print(x: i32) {
    println("{}", x)
}

func print(x: i32, y: i32) {
    println("{}, {}", x, y)
}

// Overload based on type (through generics + traits)
func max<T: Ord>(a: T, b: T) -> T {
    if a > b { a } else { b }
}
```

### [ ] Higher-Order Functions
```nova
// Function taking another function as parameter
func apply_twice(f: fn(i32) -> i32, x: i32) -> i32 {
    f(f(x))
}

func increment(x: i32) -> i32 {
    x + 1
}

let result = apply_twice(increment, 5)  // 7
```


### [ ] Closures
```nova
// Closures (anonymous functions)
let add_one = |x| x + 1
let sum = |a, b| a + b

// Closures can capture environment
let factor = 10
let multiply_by_factor = |x| x * factor

let result = multiply_by_factor(5)  // 50

// Multi-line closures
let process = |x| {
    let temp = x * 2
    let result = temp + 10
    result
}
```
---

<a id="control-flow"></a>
## Control Flow

### [ ] If Expressions
```nova
// If as statement
let number = 6
if number % 2 == 0 {
    println("even")
} else {
    println("odd")
}

// If as expression (returns value)
let result = if number > 5 {
    "big"
} else {
    "small"
}

// If-else if-else chain
let grade = if score >= 90 {
    'A'
} else if score >= 80 {
    'B'
} else if score >= 70 {
    'C'
} else {
    'F'
}
```
### [ ] Loops
```nova
// Loop (infinite)
loop {
    println("Forever!")
    break  // Exit loop
}

// While loop
let mut counter = 0
while counter < 10 {
    println("{}", counter)
    counter += 1
}

// For loop (range)
for i in 0..10 {
    println("{}", i)  // 0 to 9
}

// For loop (inclusive range)
for i in 0..=10 {
    println("{}", i)  // 0 to 10
}

// For loop (iterator)
let numbers = [1, 2, 3, 4, 5]
for n in numbers {
    println("{}", n)
}

// Loop with labels
'outer: for i in 0..3 {
    'inner: for j in 0..3 {
        if i == 1 && j == 1 {
            break 'outer  // Break outer loop
        }
        println("({}, {})", i, j)
    }
}

// Loop returning value
let result = loop {
    counter += 1
    if counter == 10 {
        break counter * 2  // Return value from loop
    }
}
```

### [ ] Continue Statement
```nova
for i in 0..10 {
    if i % 2 == 0 {
        continue  // Skip even numbers
    }
    println("{}", i)
}
```

---

<a id="ownership-borrowing"></a>
## Ownership & Borrowing

### [ ] Ownership Rules
```nova
// Rule 1: Each value has an owner
let s1 = String::from("hello")  // s1 owns the string

// Rule 2: Only one owner at a time
let s2 = s1                     // s1 moved to s2
// println("{}", s1)            // ERROR: s1 no longer valid

// Rule 3: Value dropped when owner goes out of scope
{
    let s = String::from("temp")
    // s is valid here
}  // s goes out of scope and is dropped
```


### [ ] Move Semantics
```nova
func take_ownership(s: String) {
    println("{}", s)
}  // s is dropped here

let s = String::from("hello")
take_ownership(s)
// println("{}", s)  // ERROR: s was moved
```

### [ ] Borrowing (References)
```nova
// Immutable borrow
func calculate_length(s: &String) -> usize {
    s.len()  // Can read but not modify
}

let s1 = String::from("hello")
let len = calculate_length(&s1)  // Borrow s1
println("Length of '{}' is {}", s1, len)  // s1 still valid

// Mutable borrow
func append_world(s: &mut String) {
    s.push_str(", world")
}

let mut s = String::from("hello")
append_world(&mut s)
println("{}", s)  // "hello, world"
```

### [ ] Borrowing Rules
```nova
// Can have multiple immutable borrows
let s = String::from("hello")
let r1 = &s
let r2 = &s
let r3 = &s
println("{}, {}, {}", r1, r2, r3)  // OK

// Cannot have mutable and immutable borrows simultaneously
let mut s = String::from("hello")
let r1 = &s
// let r2 = &mut s  // ERROR: cannot borrow as mutable
println("{}", r1)

// Can have ONE mutable borrow
let mut s = String::from("hello")
let r1 = &mut s
// let r2 = &mut s  // ERROR: cannot borrow twice
r1.push_str(" world")

// Borrows must not outlive the data
func dangle() -> &String {
    let s = String::from("hello")
    &s  // ERROR: returns reference to local variable
}
```

### [ ] References vs Raw Pointers
```nova
// Safe reference (ownership checked)
let x = 5
let r = &x  // Immutable reference

// Raw pointer (unsafe)
unsafe {
    let r = &x as *const i32
    let mut_r = &mut x as *mut i32
    println("{}", *r)  // Dereference in unsafe block
}
```

---

<a id="lifetimes"></a>
## Lifetimes

### [ ] Lifetime Annotations
```nova
// Lifetime annotation syntax
// 'a is a lifetime parameter

// Function with lifetime parameter
func longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}

// The returned reference is valid as long as both inputs are valid
let s1 = String::from("long string")
let s2 = String::from("short")
let result = longest(&s1, &s2)
```

### [ ] Lifetime in Structs
```nova
// Struct holding a reference needs lifetime annotation
struct Excerpt<'a> {
    part: &'a str
}

let novel = String::from("Call me Ishmael. Some years ago...")
let first_sentence = novel.split('.').next().unwrap()
let excerpt = Excerpt { part: first_sentence }

// Excerpt cannot outlive the string it references
```

### [ ] Lifetime Elision Rules
```nova
// Compiler can infer lifetimes in common cases

// Rule 1: Each reference parameter gets its own lifetime
func first_word(s: &str) -> &str  // Compiler infers: func first_word<'a>(s: &'a str) -> &'a str

// Rule 2: If there is exactly one input lifetime, it is assigned to all output lifetimes
func get_ref(s: &String) -> &str  // Output lifetime matches input

// Rule 3: If one of the parameters is &self or &mut self, its lifetime is assigned to output
impl MyStruct {
    func get_part(&self) -> &str {  // Output lifetime is 'self
        &self.data
    }
}
```

### [ ] Static Lifetime
```nova
// 'static lifetime - lives for entire program duration
let s: &'static str = "I have a static lifetime"

// String literals always have 'static lifetime
const MESSAGE: &'static str = "Hello, World!"

// Generic with static bound
func print_static<T: 'static>(t: T) {
    println("{:?}", t)
}
```

### [ ] Multiple Lifetimes
```nova
// Multiple lifetime parameters
func complex<'a, 'b>(x: &'a str, y: &'b str) -> &'a str {
    x  // Returns reference with lifetime 'a
}

// Lifetime bounds
func longer_than<'a, 'b: 'a>(x: &'a str, y: &'b str) -> &'a str {
    if x.len() > y.len() { x } else { y }
}
// 'b: 'a means 'b outlives 'a
```

---

<a id="type-system-inference"></a>
## Type System & Inference

### [ ] Type Annotations
```nova
// Explicit type annotations
let x: i32 = 5
let y: f64 = 3.14
let flag: bool = true

// Type inference
let x = 5          // i32 inferred
let y = 3.14       // f64 inferred
let flag = true    // bool inferred
```

### [ ] Compound Types
```nova
// Tuples (heterogeneous, fixed size)
let tuple: (i32, f64, bool) = (42, 3.14, true)
let (x, y, z) = tuple  // Destructuring
let first = tuple.0    // Access by index

// Arrays (homogeneous, fixed size)
let array: [i32; 5] = [1, 2, 3, 4, 5]
let first = array[0]
let length = array.len()

// Array initialization with same value
let zeros = [0; 100]  // [0, 0, 0, ..., 0] (100 elements)

// Slices (view into array)
let slice: &[i32] = &array[1..4]  // [2, 3, 4]
```


### [ ] Type Aliases
```nova
// Type alias
type Kilometers = i32
type Point = (i32, i32)

let distance: Kilometers = 100
let origin: Point = (0, 0)
```
### [ ] Never Type
```nova
// Never type (!) - function never returns
func panic_now() -> ! {
    panic("This function never returns")
}

func infinite_loop() -> ! {
    loop {
        // Never exits
    }
}
```
---

<a id="generics"></a>
## Generics

### [ ] Generic Functions
```nova
// Generic function
func identity<T>(x: T) -> T {
    x
}

let num = identity(42)      // T = i32
let text = identity("hi")   // T = str

// Generic function with multiple type parameters
func pair<T, U>(first: T, second: U) -> (T, U) {
    (first, second)
}

let p = pair(42, "answer")  // (i32, str)

// Generic function with constraints
func max<T: Ord>(a: T, b: T) -> T {
    if a > b { a } else { b }
}
```


### [ ] Generic Structs
```nova
// Generic struct
struct Point<T> {
    x: T,
    y: T
}

let integer_point = Point { x: 5, y: 10 }
let float_point = Point { x: 1.0, y: 4.0 }

// Multiple type parameters
struct Pair<T, U> {
    first: T,
    second: U
}

let pair = Pair { first: 5, second: "hello" }
```

### [ ] Generic Enums
```nova
// Option<T> - built-in generic enum
enum Option<T> {
    Some(T),
    None
}

let some_number = Option::Some(5)
let no_number: Option<i32> = Option::None

// Result<T, E> - built-in generic enum
enum Result<T, E> {
    Ok(T),
    Err(E)
}

let success: Result<i32, str> = Result::Ok(42)
let failure: Result<i32, str> = Result::Err("error")
```

### [ ] Generic Methods
```nova
struct Point<T> {
    x: T,
    y: T
}

impl<T> Point<T> {
    // Generic method
    func new(x: T, y: T) -> Self {
        Point { x, y }
    }
    
    func x(&self) -> &T {
        &self.x
    }
}

// Method with different generic type
impl<T> Point<T> {
    func mix_up<U>(self, other: Point<U>) -> Point<U> {
        Point {
            x: self.x,
            y: other.y
        }
    }
}
```

### [ ] Monomorphization
```nova
// Compiler generates specific versions for each type
func print_type<T>(value: T) {
    println("{}", value)
}

print_type(42)      // Generates: print_type_i32
print_type(3.14)    // Generates: print_type_f64
print_type("hi")    // Generates: print_type_str

// Zero runtime cost - fully resolved at compile time
```

---

<a id="pattern-matching"></a>
## Pattern Matching

### [ ] Match Expression
```nova
// Basic match
let number = 7

match number {
    1 => println("one"),
    2 => println("two"),
    3 | 4 | 5 => println("three, four, or five"),
    6..=10 => println("six through ten"),
    _ => println("something else")
}

// Match with return value
let result = match number {
    1 => "one",
    2 => "two",
    _ => "many"
}

// Match on multiple values
match (x, y) {
    (0, 0) => println("origin"),
    (0, _) => println("on y-axis"),
    (_, 0) => println("on x-axis"),
    (_, _) => println("elsewhere")
}
```

### [ ] Destructuring in Patterns
```nova
// Destructure structs
struct Point { x: i32, y: i32 }

let point = Point { x: 10, y: 20 }

match point {
    Point { x: 0, y: 0 } => println("origin"),
    Point { x: 0, y } => println("on y-axis at {}", y),
    Point { x, y: 0 } => println("on x-axis at {}", x),
    Point { x, y } => println("at ({}, {})", x, y)
}

// Destructure enums
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32)
}

let msg = Message::Move { x: 10, y: 20 }

match msg {
    Message::Quit => println("quit"),
    Message::Move { x, y } => println("move to ({}, {})", x, y),
    Message::Write(text) => println("text: {}", text),
    Message::ChangeColor(r, g, b) => println("rgb({}, {}, {})", r, g, b)
}
```

### [ ] Match Guards
```nova
let number = 4

match number {
    n if n < 0 => println("negative"),
    n if n == 0 => println("zero"),
    n if n % 2 == 0 => println("even positive"),
    _ => println("odd positive")
}

// Multiple patterns with guard
match number {
    1 | 2 | 3 if number > 1 => println("2 or 3"),
    _ => println("other")
}
```

### [ ] If Let and While Let
```nova
// If let (shorthand for single pattern match)
let some_value = Option::Some(5)

if let Option::Some(x) = some_value {
    println("got value: {}", x)
} else {
    println("no value")
}

// While let (loop until pattern does not match)
let mut stack = vec![1, 2, 3]

while let Option::Some(top) = stack.pop() {
    println("{}", top)
}
```

### [ ] Pattern Matching in Function Parameters
```nova
// Destructure in function parameters
func print_point((x, y): (i32, i32)) {
    println("Point at ({}, {})", x, y)
}

print_point((10, 20))

// Match in closure parameters
let pairs = vec![(1, 2), (3, 4), (5, 6)]
pairs.iter().for_each(|(x, y)| {
    println("{} + {} = {}", x, y, x + y)
})
```

---

<a id="error-handling"></a>
## Error Handling

### [ ] Option\<T\>
```nova
// Option - represents optional value
enum Option<T> {
    Some(T),
    None
}

// Basic usage
func divide(a: i32, b: i32) -> Option<i32> {
    if b == 0 {
        Option::None
    } else {
        Option::Some(a / b)
    }
}

let result = divide(10, 2)
match result {
    Option::Some(value) => println("result: {}", value),
    Option::None => println("cannot divide by zero")
}

// Option methods
let x = Option::Some(5)
let y = x.unwrap()              // Panics if None
let z = x.unwrap_or(0)          // Returns 0 if None
let w = x.unwrap_or_else(|| 10) // Calls function if None
let is_some = x.is_some()       // true
let is_none = x.is_none()       // false
```


### [ ] Result\<T, E\>
```nova
// Result - represents success or failure
enum Result<T, E> {
    Ok(T),
    Err(E)
}

// Basic usage
func parse_number(s: str) -> Result<i32, String> {
    match s.parse::<i32>() {
        Ok(num) => Result::Ok(num),
        Err(_) => Result::Err(String::from("invalid number"))
    }
}

let result = parse_number("42")
match result {
    Result::Ok(num) => println("parsed: {}", num),
    Result::Err(e) => println("error: {}", e)
}

// Result methods
let x = Result::Ok(5)
let y = x.unwrap()              // Panics if Err
let z = x.expect("failed")      // Panics with custom message
let is_ok = x.is_ok()           // true
let is_err = x.is_err()         // false
```

### [ ] Panic (Unrecoverable Errors)
```nova
// panic! - terminate program
func divide(a: i32, b: i32) -> i32 {
    if b == 0 {
        panic("division by zero")
    }
    a / b
}

// Custom panic handler
func safe_divide(a: i32, b: i32) -> i32 {
    if b == 0 {
        panic("Cannot divide {} by zero", a)
    }
    a / b
}
```

### [ ] Custom Error Types
```nova
// Custom error enum
enum MathError {
    DivisionByZero,
    NegativeSquareRoot,
    Overflow
}

func divide_safe(a: i32, b: i32) -> Result<i32, MathError> {
    if b == 0 {
        Result::Err(MathError::DivisionByZero)
    } else {
        Result::Ok(a / b)
    }
}

// Error with data
enum FileError {
    NotFound(String),
    PermissionDenied,
    IOError(i32)
}

func open_file(path: str) -> Result<File, FileError> {
    // ...
}
```

### [ ] Error Propagation (`?` Operator)
```nova
// The ? operator propagates errors automatically
func read_username_from_file() -> Result<String, IOError> {
    let mut file = File::open("username.txt")?  // Returns early if Err
    let mut username = String::new()
    file.read_to_string(&mut username)?         // Returns early if Err
    Ok(username)
}

// Equivalent to:
func read_username_verbose() -> Result<String, IOError> {
    let file_result = File::open("username.txt")
    let mut file = match file_result {
        Ok(f) => f,
        Err(e) => return Err(e)
    }
    let mut username = String::new()
    match file.read_to_string(&mut username) {
        Ok(_) => Ok(username),
        Err(e) => Err(e)
    }
}

// Chaining with ?
func get_user_data() -> Result<UserData, Error> {
    let config = read_config()?
    let user_id = config.get_user_id()?
    let user = fetch_user(user_id)?
    Ok(user)
}

// Using ? with Option
func get_first_char(s: &str) -> Option<char> {
    s.lines().next()?.chars().next()
}

// Converting errors with ?
func read_number() -> Result<i32, String> {
    let content = fs::read_to_string("number.txt")
        .map_err(|e| e.to_string())?
    let number = content.trim().parse::<i32>()
        .map_err(|e| e.to_string())?
    Ok(number)
}
```

### [ ] Error Handling Best Practices
```nova
// Use Result for recoverable errors
func validate_age(age: i32) -> Result<i32, String> {
    if age < 0 {
        Err(String::from("Age cannot be negative"))
    } else if age > 150 {
        Err(String::from("Age seems unrealistic"))
    } else {
        Ok(age)
    }
}

// Use Option for optional values
func find_user(id: i32) -> Option<User> {
    // Returns None if not found, Some(user) if found
}

// Use panic! only for unrecoverable errors
func initialize() {
    let config = load_config().expect("Config file is required")
    // If config fails to load, the program cannot continue
}

// Provide context with error messages
func process_file(path: &str) -> Result<Data, ProcessError> {
    let file = File::open(path)
        .map_err(|e| ProcessError::FileOpen { path: path.to_string(), source: e })?
    // ...
}
```

---

<a id="object-oriented-programming"></a>
## Object-Oriented Programming

### [ ] Structs
```nova
// Basic struct
struct Person {
    name: String,
    age: u32,
    email: String
}

// Create instance
let person = Person {
    name: String::from("Alice"),
    age: 30,
    email: String::from("alice@example.com")
}

// Access fields
println("{} is {} years old", person.name, person.age)

// Struct update syntax
let person2 = Person {
    name: String::from("Bob"),
    ..person  // Copy remaining fields from person
}

// Tuple structs
struct Color(u8, u8, u8)
let black = Color(0, 0, 0)

// Unit struct (no fields)
struct Empty
```


### [ ] Methods
```nova
struct Rectangle {
    width: u32,
    height: u32
}

impl Rectangle {
    // Associated function (constructor)
    pub func new(width: u32, height: u32) -> Self {
        Rectangle { width, height }
    }
    
    // Method (takes self)
    pub func area(&self) -> u32 {
        self.width * self.height
    }
    
    // Method with mutable borrow
    pub func scale(&mut self, factor: u32) {
        self.width *= factor
        self.height *= factor
    }
    
    // Method taking ownership
    pub func into_square(self) -> Square {
        let size = self.width.max(self.height)
        Square { size }
    }
}

// Usage
let rect = Rectangle::new(10, 20)
let area = rect.area()

let mut rect2 = Rectangle::new(5, 10)
rect2.scale(2)  // Now 10x20
```

### [ ] Classes (Inheritance)
```nova
// Base class
class Animal {
    name: String,
    age: u32
    
    pub func new(name: String, age: u32) -> Self {
        Animal { name, age }
    }
    
    pub func speak(&self) -> String {
        "Some sound"
    }
    
    pub func describe(&self) {
        println("{} is {} years old", self.name, self.age)
    }
}

// Derived class
class Dog : Animal {
    breed: String
    
    pub func new(name: String, age: u32, breed: String) -> Self {
        Dog {
            name,
            age,
            breed
        }
    }
    
    // Override method
    override func speak(&self) -> String {
        "Woof!"
    }
    
    // New method
    pub func fetch(&self) {
        println("{} is fetching", self.name)
    }
}

// Usage
let dog = Dog::new(String::from("Buddy"), 3, String::from("Golden Retriever"))
println("{}", dog.speak())  // "Woof!"
dog.describe()              // Inherited method
dog.fetch()                 // Dog-specific method

// Polymorphism
let animal: &Animal = &dog  // Upcast
println("{}", animal.speak())  // Calls Dog::speak
```

### [ ] Visibility
```nova
// Public struct with private fields
pub struct User {
    pub username: String,    // Public field
    email: String,           // Private field (default)
    priv password_hash: String  // Explicitly private
}

impl User {
    pub func new(username: String, email: String) -> Self {
        User {
            username,
            email,
            password_hash: String::from("hashed")
        }
    }
    
    // Public getter
    pub func email(&self) -> &str {
        &self.email
    }
    
    // Private method
    priv func verify_password(&self, password: str) -> bool {
        // Implementation
        true
    }
}
```
---

<a id="traits-interfaces"></a>
## Traits & Interfaces

### [ ] Defining Traits
```nova
// Basic trait
trait Speak {
    func speak(&self) -> String;
}

// Trait with default implementation
trait Greet {
    func greet(&self) -> String {
        String::from("Hello")
    }
    
    func say_goodbye(&self) -> String {
        String::from("Goodbye")
    }
}

// Trait with associated types
trait Iterator {
    type Item;
    
    func next(&mut self) -> Option<Self::Item>;
}

// Trait with associated constants
trait MathConstants {
    const PI: f64 = 3.14159265359;
    const E: f64 = 2.71828182846;
}
```

### [ ] Implementing Traits
```nova
struct Dog {
    name: String
}

impl Speak for Dog {
    func speak(&self) -> String {
        String::from("Woof!")
    }
}

impl Greet for Dog {
    // Use default implementation for greet
    // Override say_goodbye
    func say_goodbye(&self) -> String {
        format("{} says goodbye", self.name)
    }
}

// Usage
let dog = Dog { name: String::from("Buddy") }
println("{}", dog.speak())        // "Woof!"
println("{}", dog.greet())        // "Hello" (default)
println("{}", dog.say_goodbye())  // "Buddy says goodbye"
```

### [ ] Trait Bounds
```nova
// Function with trait bound
func print_speak<T: Speak>(item: &T) {
    println("{}", item.speak())
}

// Multiple trait bounds
func print_all<T: Speak + Greet>(item: &T) {
    println("{}", item.speak())
    println("{}", item.greet())
}

// Where clause (more readable for complex bounds)
func complex<T, U>(t: &T, u: &U) -> i32
    where T: Speak + Greet,
          U: Speak + Clone
{
    // Implementation
    42
}
```

### [ ] Built-in Traits
```nova
// Clone - create deep copy
#[derive(Clone)]
struct Point {
    x: i32,
    y: i32
}

let p1 = Point { x: 1, y: 2 }
let p2 = p1.clone()

// Copy - implicit copy instead of move
#[derive(Copy, Clone)]
struct SmallData {
    value: i32
}

let x = SmallData { value: 5 }
let y = x  // Copied, not moved
println("{}", x.value)  // Still valid

// Debug - format for debugging
#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32
}

let rect = Rectangle { width: 10, height: 20 }
println("{:?}", rect)  // Rectangle { width: 10, height: 20 }

// PartialEq - equality comparison
#[derive(PartialEq)]
struct Point {
    x: i32,
    y: i32
}

let p1 = Point { x: 1, y: 2 }
let p2 = Point { x: 1, y: 2 }
assert(p1 == p2)

// Ord - ordering comparison
#[derive(Eq, PartialEq, Ord, PartialOrd)]
struct Person {
    name: String,
    age: u32
}
```

### [ ] Trait Objects (Dynamic Dispatch)
```nova
// Trait object - runtime polymorphism
trait Draw {
    func draw(&self);
}

struct Circle {
    radius: f64
}

impl Draw for Circle {
    func draw(&self) {
        println("Drawing circle with radius {}", self.radius)
    }
}

struct Rectangle {
    width: f64,
    height: f64
}

impl Draw for Rectangle {
    func draw(&self) {
        println("Drawing rectangle {}x{}", self.width, self.height)
    }
}

// Vector of trait objects
let shapes: Vec<Box<dyn Draw>> = vec![
    Box::new(Circle { radius: 5.0 }),
    Box::new(Rectangle { width: 10.0, height: 20.0 })
]

for shape in shapes {
    shape.draw()  // Dynamic dispatch
}
```
---

<a id="smart-pointers"></a>
## Smart Pointers

### [ ] Box\<T\> (Heap Allocation)
```nova
// Box - allocate data on the heap
let boxed = Box::new(5)
println("Value: {}", *boxed)  // Dereference to access value

// Useful for recursive types
enum List {
    Cons(i32, Box<List>),
    Nil
}

let list = List::Cons(1, 
    Box::new(List::Cons(2, 
        Box::new(List::Cons(3, 
            Box::new(List::Nil))))));

// Box for trait objects
let drawable: Box<dyn Draw> = Box::new(Circle { radius: 5.0 })
drawable.draw()
```

### [ ] Rc\<T\> (Reference Counting)
```nova
use std::rc::Rc

// Rc - multiple ownership through reference counting
let data = Rc::new(vec![1, 2, 3])

let clone1 = Rc::clone(&data)  // Increment reference count
let clone2 = Rc::clone(&data)  // Increment again

println("Reference count: {}", Rc::strong_count(&data))  // 3

// When all Rc instances go out of scope, data is dropped

// Shared ownership example
struct Node {
    value: i32,
    parent: Option<Rc<Node>>
}

let parent = Rc::new(Node { value: 1, parent: None })
let child1 = Node { value: 2, parent: Some(Rc::clone(&parent)) }
let child2 = Node { value: 3, parent: Some(Rc::clone(&parent)) }
```

### [ ] RefCell\<T\> (Interior Mutability)
```nova
use std::cell::RefCell

// RefCell - mutable borrows checked at runtime
let data = RefCell::new(5)

// Borrow mutably even though RefCell is not mut
*data.borrow_mut() += 1
println("Value: {}", *data.borrow())  // 6

// Multiple immutable borrows OK
let r1 = data.borrow()
let r2 = data.borrow()

// But this would panic at runtime:
// let r1 = data.borrow()
// let r2 = data.borrow_mut()  // PANIC: already borrowed

// Common pattern: Rc<RefCell<T>> for shared mutable data
let shared = Rc::new(RefCell::new(vec![1, 2, 3]))
let clone1 = Rc::clone(&shared)
let clone2 = Rc::clone(&shared)

clone1.borrow_mut().push(4)
clone2.borrow_mut().push(5)
// shared now contains [1, 2, 3, 4, 5]
```

### [ ] Weak\<T\> (Weak References)
```nova
use std::rc::{Rc, Weak}

// Weak - non-owning reference, does not prevent deallocation
struct Node {
    value: i32,
    parent: RefCell<Weak<Node>>,
    children: RefCell<Vec<Rc<Node>>>
}

// Prevents reference cycles that would cause memory leaks
let parent = Rc::new(Node {
    value: 1,
    parent: RefCell::new(Weak::new()),
    children: RefCell::new(vec![])
})

let child = Rc::new(Node {
    value: 2,
    parent: RefCell::new(Rc::downgrade(&parent)),  // Weak reference
    children: RefCell::new(vec![])
})

parent.children.borrow_mut().push(Rc::clone(&child))

// Access weak reference
if let Some(p) = child.parent.borrow().upgrade() {
    println("Parent value: {}", p.value)
}
```

### [ ] Cell\<T\> (Copy Types Interior Mutability)
```nova
use std::cell::Cell

// Cell - interior mutability for Copy types
let cell = Cell::new(5)

cell.set(10)                    // No borrow needed
let value = cell.get()          // Returns copy, not reference

// Useful in structs
struct Counter {
    count: Cell<i32>
}

impl Counter {
    func increment(&self) {     // &self, not &mut self
        self.count.set(self.count.get() + 1)
    }
}
```

---

<a id="functional-programming"></a>
## Functional Programming

### [ ] First-Class Functions
```nova
// Function as value
let add = |a, b| a + b
let result = add(5, 3)

// Function type annotation
let multiply: fn(i32, i32) -> i32 = |a, b| a * b

// Return function from function
func make_adder(n: i32) -> fn(i32) -> i32 {
    move |x| x + n  // Closure captures n
}

let add_five = make_adder(5)
let result = add_five(10)  // 15
```

### [ ] Closures (Captures)
```nova
// Capture by reference
let x = 10
let print_x = || println("{}", x)  // Borrows x
print_x()

// Capture by mutable reference
let mut y = 10
let mut increment = || {
    y += 1
    println("{}", y)
}
increment()  // 11
increment()  // 12

// Capture by move
let s = String::from("hello")
let consume = move || println("{}", s)  // Takes ownership
consume()
// println("{}", s)  // ERROR: s was moved

// Closure with explicit parameter types
let add = |a: i32, b: i32| -> i32 { a + b }
```

### [ ] Higher-Order Functions with Collections
```nova
// Map - transform each element
let numbers = [1, 2, 3, 4, 5]
let doubled = numbers.iter().map(|x| x * 2).collect()  // [2, 4, 6, 8, 10]

// Filter - keep elements matching predicate
let evens = numbers.iter().filter(|x| x % 2 == 0).collect()  // [2, 4]

// Fold/Reduce - accumulate into single value
let sum = numbers.iter().fold(0, |acc, x| acc + x)  // 15

// Chaining operations
let result = numbers.iter()
    .filter(|x| x % 2 == 1)
    .map(|x| x * x)
    .fold(0, |acc, x| acc + x)  // 1 + 9 + 25 = 35
```

---

<a id="module-system"></a>
## Module System

### [ ] Defining Modules
```nova
// In file: lib.nova
mod math {
    pub func add(a: i32, b: i32) -> i32 {
        a + b
    }
    
    // Private by default
    func helper() -> i32 {
        42
    }
    
    // Nested modules
    pub mod advanced {
        pub func power(base: i32, exp: i32) -> i32 {
            let mut result = 1
            for _ in 0..exp {
                result *= base
            }
            result
        }
    }
}
```

### [ ] Using Modules
```nova
// Import entire module
use math

let sum = math::add(1, 2)

// Import specific items
use math::add
use math::advanced::power

let sum = add(1, 2)
let squared = power(2, 8)

// Import with alias
use math::advanced as adv

let result = adv::power(2, 10)

// Glob import (use sparingly)
use math::*
```

### [ ] File-Based Modules
```nova
// File structure:
// src/
//   main.nova
//   utils.nova
//   math/
//     mod.nova
//     geometry.nova

// In main.nova:
mod utils           // Loads src/utils.nova
mod math            // Loads src/math/mod.nova

use utils::helper
use math::geometry::Circle
```

### [ ] Re-exports
```nova
mod internal {
    pub mod types {
        pub struct Config {
            pub name: String,
            pub value: i32
        }
    }
}

// Re-export for cleaner API
pub use internal::types::Config
```

---

<a id="collections"></a>
## Collections

### [ ] Vec (Dynamic Array)
```nova
// Create vectors
let mut numbers: Vec<i32> = Vec::new()
let numbers = vec![1, 2, 3, 4, 5]

// Add elements
numbers.push(6)
numbers.push(7)

// Access elements
let first = numbers[0]         // Panics if out of bounds
let maybe = numbers.get(0)     // Returns Option<&i32>

// Remove elements
let last = numbers.pop()       // Returns Option<i32>
let second = numbers.remove(1) // Remove at index

// Iteration
for n in &numbers {
    println("{}", n)
}

// Length and capacity
let len = numbers.len()
let cap = numbers.capacity()
numbers.reserve(100)  // Pre-allocate
```

### [ ] HashMap
```nova
use collections::HashMap

// Create hashmap
let mut scores: HashMap<String, i32> = HashMap::new()

// Insert
scores.insert(String::from("Alice"), 100)
scores.insert(String::from("Bob"), 85)

// Access
let alice_score = scores.get(&String::from("Alice"))  // Option<&i32>

// Update
scores.insert(String::from("Alice"), 95)  // Overwrites

// Entry API
scores.entry(String::from("Charlie")).or_insert(0)

// Iteration
for (name, score) in &scores {
    println("{}: {}", name, score)
}
```

### [ ] HashSet
```nova
use collections::HashSet

let mut seen: HashSet<i32> = HashSet::new()

// Add elements
seen.insert(1)
seen.insert(2)
seen.insert(1)  // Duplicate, ignored

// Check membership
if seen.contains(&1) {
    println("Found 1")
}

// Set operations
let a: HashSet<i32> = [1, 2, 3].into()
let b: HashSet<i32> = [2, 3, 4].into()

let union = a.union(&b)          // {1, 2, 3, 4}
let intersection = a.intersection(&b)  // {2, 3}
let difference = a.difference(&b)      // {1}
```

---

<a id="iterators"></a>
## Iterators

### [ ] Iterator Trait
```nova
trait Iterator {
    type Item
    
    func next(&mut self) -> Option<Self::Item>
}
```

### [ ] Creating Iterators
```nova
// From collections
let numbers = [1, 2, 3, 4, 5]
let iter = numbers.iter()       // Iterator over &i32
let iter_mut = numbers.iter_mut()  // Iterator over &mut i32
let into_iter = numbers.into_iter() // Takes ownership

// Ranges
let range = 0..10       // 0 to 9
let inclusive = 0..=10  // 0 to 10
```

### [ ] Iterator Adapters
```nova
let numbers = vec![1, 2, 3, 4, 5]

// map - transform elements
let squared = numbers.iter().map(|x| x * x)

// filter - keep matching elements
let evens = numbers.iter().filter(|x| *x % 2 == 0)

// take - limit number of elements
let first_three = numbers.iter().take(3)

// skip - skip first n elements
let after_two = numbers.iter().skip(2)

// enumerate - add index
for (i, n) in numbers.iter().enumerate() {
    println("{}: {}", i, n)
}

// zip - combine two iterators
let a = [1, 2, 3]
let b = [4, 5, 6]
let pairs = a.iter().zip(b.iter())  // [(1,4), (2,5), (3,6)]

// flatten - flatten nested iterators
let nested = vec![vec![1, 2], vec![3, 4]]
let flat = nested.iter().flatten()  // [1, 2, 3, 4]

// chain - concatenate iterators
let combined = a.iter().chain(b.iter())
```

### [ ] Consuming Iterators
```nova
let numbers = vec![1, 2, 3, 4, 5]

// collect - gather into collection
let doubled: Vec<i32> = numbers.iter().map(|x| x * 2).collect()

// sum - add all elements
let total: i32 = numbers.iter().sum()

// product - multiply all elements
let factorial: i32 = (1..=5).product()  // 120

// count - count elements
let count = numbers.iter().count()

// find - first matching element
let first_even = numbers.iter().find(|x| *x % 2 == 0)

// any/all - check conditions
let has_even = numbers.iter().any(|x| x % 2 == 0)  // true
let all_positive = numbers.iter().all(|x| *x > 0)   // true

// min/max
let minimum = numbers.iter().min()
let maximum = numbers.iter().max()
```

---

<a id="concurrency"></a>
## Concurrency

### [ ] Threads
```nova
use std::thread

// Spawn a new thread
let handle = thread::spawn(|| {
    println("Hello from thread!")
})

// Wait for thread to complete
handle.join()

// Move data into thread
let data = vec![1, 2, 3]
let handle = thread::spawn(move || {
    println("Data: {:?}", data)
})
```

### [ ] Message Passing
```nova
use std::sync::mpsc

// Create channel
let (sender, receiver) = mpsc::channel()

thread::spawn(move || {
    sender.send(42)
})

let received = receiver.recv()  // Blocks until message received
println("Got: {}", received)

// Multiple producers
let (tx, rx) = mpsc::channel()
for i in 0..5 {
    let tx_clone = tx.clone()
    thread::spawn(move || {
        tx_clone.send(i)
    })
}

for received in rx {
    println("Got: {}", received)
}
```

### [ ] Shared State
```nova
use std::sync::{Arc, Mutex}

// Arc - atomic reference counting
// Mutex - mutual exclusion

let counter = Arc::new(Mutex::new(0))

let mut handles = vec![]

for _ in 0..10 {
    let counter = Arc::clone(&counter)
    let handle = thread::spawn(move || {
        let mut num = counter.lock()
        *num += 1
    })
    handles.push(handle)
}

for handle in handles {
    handle.join()
}

println("Result: {}", *counter.lock())  // 10
```

### [ ] Async/Await
```nova
// Async function
async func fetch_data(url: str) -> Result<String, Error> {
    let response = http::get(url).await?
    response.text().await
}

// Running async code
async func main() {
    let result = fetch_data("https://example.com").await
    match result {
        Ok(data) => println("{}", data),
        Err(e) => println("Error: {}", e)
    }
}

// Concurrent execution
async func fetch_all() {
    let (a, b) = join!(
        fetch_data("https://api1.com"),
        fetch_data("https://api2.com")
    )
}
```

---

<a id="unsafe-code"></a>
## Unsafe Code

### [ ] Unsafe Blocks
```nova
// Unsafe operations require unsafe block
unsafe {
    // Dereference raw pointers
    let x = 5
    let ptr = &x as *const i32
    println("Value: {}", *ptr)
    
    // Call unsafe functions
    dangerous_function()
    
    // Access mutable statics
    COUNTER += 1
}
```

### [ ] Raw Pointers
```nova
let x = 5
let y = &mut x

// Create raw pointers (safe)
let ptr_const: *const i32 = &x
let ptr_mut: *mut i32 = y

// Dereference (unsafe)
unsafe {
    println("const ptr: {}", *ptr_const)
    *ptr_mut = 10
}
```

### [ ] Unsafe Functions
```nova
unsafe func dangerous() {
    // Implementation that requires manual safety guarantees
}

func safe_wrapper() {
    // ... safe setup ...
    unsafe {
        dangerous()
    }
    // ... safe cleanup ...
}
```

### [ ] Foreign Function Interface (FFI)
```nova
// Declare external C function
extern "C" {
    func abs(input: i32) -> i32
    func strlen(s: *const i8) -> usize
}

// Call external function
func main() {
    unsafe {
        let result = abs(-5)
        println("Absolute value: {}", result)
    }
}

// Export Nova function to C
#[no_mangle]
pub extern "C" func call_from_c() {
    println("Called from C!")
}
```

---

<a id="macros"></a>
## Macros

### [ ] Declarative Macros
```nova
// Simple macro
macro vec!($($x:expr),*) {
    {
        let mut temp_vec = Vec::new()
        $(
            temp_vec.push($x)
        )*
        temp_vec
    }
}

let numbers = vec![1, 2, 3, 4, 5]

// Macro with different patterns
macro say_hello!() {
    println("Hello!")
}

macro say_hello!($name:expr) {
    println("Hello, {}!", $name)
}

say_hello!()           // "Hello!"
say_hello!("Alice")    // "Hello, Alice!"
```

### [ ] Built-in Macros
```nova
// println - formatted printing
println("Hello, {}!", name)
println("{:?}", debug_value)     // Debug format
println("{:#?}", pretty_debug)   // Pretty debug

// format - create formatted string
let s = format!("Value: {}", 42)

// assert - runtime assertion
assert!(condition)
assert_eq!(a, b)
assert_ne!(a, b)

// panic - abort with message
panic!("Something went wrong!")

// dbg - debug printing with location
let x = dbg!(5 * 2)  // Prints "[file:line] 5 * 2 = 10"

// todo/unimplemented
func work_in_progress() {
    todo!("Implement this function")
}
```

---

<a id="attributes"></a>
## Attributes

### [ ] Common Attributes
```nova
// Derive implementations
#[derive(Debug, Clone, PartialEq)]
struct Point {
    x: i32,
    y: i32
}

// Conditional compilation
#[cfg(target_os = "linux")]
func linux_only() {
    // Only compiled on Linux
}

#[cfg(test)]
mod tests {
    // Only compiled during testing
}

// Allow/deny lints
#[allow(unused_variables)]
func example() {
    let x = 5  // No warning
}

#[deny(dead_code)]
mod strict_module {
    // Unused code is an error
}

// Documentation
/// This is a doc comment for the function
/// 
/// # Examples
/// ```
/// let result = add(1, 2)
/// assert_eq!(result, 3)
/// ```
pub func add(a: i32, b: i32) -> i32 {
    a + b
}

// Inline hint
#[inline]
func small_function() -> i32 {
    42
}

#[inline(always)]
func force_inline() -> i32 {
    42
}

// Test function
#[test]
func test_addition() {
    assert_eq!(add(2, 2), 4)
}

// Mark as deprecated
#[deprecated(since = "1.0.0", note = "Use new_function instead")]
func old_function() {
    // ...
}
```

---

<a id="testing"></a>
## Testing

### [ ] Unit Tests
```nova
// Unit tests are placed in the same file as the code
#[cfg(test)]
mod tests {
    use super::*
    
    #[test]
    func test_addition() {
        assert_eq!(add(2, 2), 4)
    }
    
    #[test]
    func test_subtraction() {
        let result = subtract(10, 5)
        assert_eq!(result, 5)
    }
    
    #[test]
    #[should_panic]
    func test_divide_by_zero() {
        divide(10, 0)  // Should panic
    }
    
    #[test]
    #[should_panic(expected = "division by zero")]
    func test_panic_message() {
        divide(10, 0)  // Checks panic message
    }
}
```

### [ ] Test Assertions
```nova
#[test]
func test_assertions() {
    // Basic assertion
    assert!(true)
    assert!(1 + 1 == 2)
    
    // Equality assertions
    assert_eq!(4, 2 + 2)
    assert_ne!(4, 2 + 1)
    
    // With custom message
    assert!(result > 0, "Result should be positive, got {}", result)
    assert_eq!(a, b, "Values should match: {} vs {}", a, b)
    
    // Approximate equality for floats
    let a = 0.1 + 0.2
    let b = 0.3
    assert!((a - b).abs() < 1e-10, "Floats should be approximately equal")
}
```

### [ ] Test Organization
```nova
// Group related tests
mod math_tests {
    use super::*
    
    mod addition {
        use super::*
        
        #[test]
        func positive_numbers() {
            assert_eq!(add(1, 2), 3)
        }
        
        #[test]
        func negative_numbers() {
            assert_eq!(add(-1, -2), -3)
        }
    }
    
    mod subtraction {
        use super::*
        
        #[test]
        func basic() {
            assert_eq!(subtract(5, 3), 2)
        }
    }
}

// Ignore slow tests by default
#[test]
#[ignore]
func expensive_test() {
    // Only runs with: nova test -- --ignored
}
```

### [ ] Integration Tests
```nova
// Integration tests go in tests/ directory
// tests/integration_test.nova

use my_crate::*

#[test]
func test_full_workflow() {
    let config = Config::load("test_config.toml")
    let result = process_data(&config)
    assert!(result.is_ok())
}

// Setup and teardown
struct TestContext {
    temp_dir: PathBuf
}

impl TestContext {
    func new() -> Self {
        let temp_dir = create_temp_dir()
        TestContext { temp_dir }
    }
}

impl Drop for TestContext {
    func drop(&mut self) {
        remove_dir_all(&self.temp_dir)
    }
}

#[test]
func test_with_context() {
    let ctx = TestContext::new()
    // Test using ctx.temp_dir
    // Automatically cleaned up when ctx goes out of scope
}
```

### [ ] Test Results and Output
```nova
// Return Result from tests for cleaner error handling
#[test]
func test_with_result() -> Result<(), String> {
    let data = load_test_data()?
    let result = process(&data)?
    assert_eq!(result.count, 42)
    Ok(())
}

// Capture test output
#[test]
func test_with_output() {
    println("This only shows if test fails")
    // Run with: nova test -- --nocapture
    // to always see output
}
```

### [ ] Benchmarks
```nova
// Benchmark tests (if supported)
#[bench]
func bench_sort(b: &mut Bencher) {
    let mut data = generate_random_data(1000)
    b.iter(|| {
        let mut copy = data.clone()
        sort(&mut copy)
    })
}

// Simple manual timing
#[test]
func performance_test() {
    let start = Instant::now()
    
    for _ in 0..1000 {
        expensive_operation()
    }
    
    let duration = start.elapsed()
    assert!(duration < Duration::from_secs(1), 
        "Operation too slow: {:?}", duration)
}
```

---

<a id="standard-library"></a>
## Standard Library

The standard library is organized into modules under `stdlib/`:

### [ ] Core Module (Prelude)
```nova
// stdlib/core/prelude.nova - Automatically imported types
Option<T>       // Optional values (stdlib/core/option.nova)
Result<T, E>    // Error handling (stdlib/core/result.nova)
String          // Owned string (stdlib/core/string.nova)
Vec<T>          // Dynamic array (stdlib/collections/vec.nova)
Box<T>          // Heap allocation
Iterator        // Iterator trait (stdlib/core/iter.nova)

// Core traits (stdlib/core/)
Clone, Copy     // clone.nova
Default         // default.nova
Debug, Display  // fmt.nova
Eq, Ord         // cmp.nova
Add, Sub, Mul   // ops.nova
```

### [ ] Collections Module
```nova
// stdlib/collections/
Vec<T>          // vec.nova - Dynamic array
HashMap<K, V>   // hashmap.nova - Hash map
HashSet<T>      // hashset.nova - Hash set
LinkedList<T>   // linkedlist.nova - Doubly-linked list
Deque<T>        // deque.nova - Double-ended queue
```

### [ ] I/O Module
```nova
// stdlib/io/
Read            // mod.nova - Read trait
Write           // mod.nova - Write trait
File            // file.nova - File operations
stdin, stdout   // Standard streams
```

### [ ] Synchronization Module
```nova
// stdlib/sync/
Mutex<T>        // mutex.nova - Mutual exclusion
RwLock<T>       // rwlock.nova - Reader-writer lock
Arc<T>          // arc.nova - Atomic reference counting
```

### [ ] Threading and Async
```nova
// stdlib/thread/mod.nova
spawn()         // Spawn a new thread
join()          // Wait for thread completion
sleep()         // Sleep current thread

// stdlib/async/mod.nova
Future          // Future trait
async/await     // Async syntax support
```

### [ ] String Operations
```nova
// String creation
let s = String::from("Hello")
let s = "Hello".to_string()

// Concatenation
let hello = String::from("Hello, ")
let world = String::from("world!")
let greeting = hello + &world

// Formatting
let name = "Alice"
let age = 30
let intro = format("{} is {} years old", name, age)

// String methods
let s = "  Hello, World!  "
s.trim()           // "Hello, World!"
s.to_uppercase()   // "  HELLO, WORLD!  "
s.to_lowercase()   // "  hello, world!  "
s.contains("World") // true
s.replace("World", "Nova")  // "  Hello, Nova!  "
s.split(",")       // Iterator over ["  Hello", " World!  "]
```

### [ ] File I/O
```nova
use std::fs
use std::io::{Read, Write}

// Read entire file
let contents = fs::read_to_string("file.txt")?

// Write to file
fs::write("output.txt", "Hello, World!")?

// File handle
let mut file = fs::File::open("file.txt")?
let mut contents = String::new()
file.read_to_string(&mut contents)?

// Create and write
let mut file = fs::File::create("output.txt")?
file.write_all(b"Hello, World!")?

// Directory operations
fs::create_dir("new_dir")?
fs::create_dir_all("path/to/nested/dir")?
fs::remove_file("file.txt")?
fs::remove_dir("empty_dir")?
```

### [ ] Command Line
```nova
use std::env

// Command line arguments
let args: Vec<String> = env::args().collect()
let program_name = &args[0]
let first_arg = args.get(1)

// Environment variables
let path = env::var("PATH")?
env::set_var("MY_VAR", "value")

// Current directory
let cwd = env::current_dir()?
env::set_current_dir("/new/path")?
```

### [ ] Time and Duration
```nova
use std::time::{Duration, Instant}

// Measure elapsed time
let start = Instant::now()
// ... do work ...
let elapsed = start.elapsed()
println("Took {:?}", elapsed)

// Create durations
let five_seconds = Duration::from_secs(5)
let half_second = Duration::from_millis(500)

// Sleep
thread::sleep(Duration::from_secs(1))
```

### [ ] Math Operations
```nova
// Integer operations
let x: i32 = -5
x.abs()           // 5
x.pow(2)          // 25
i32::MAX          // 2147483647
i32::MIN          // -2147483648

// Floating point operations
let y: f64 = 3.14
y.floor()         // 3.0
y.ceil()          // 4.0
y.round()         // 3.0
y.sqrt()          // 1.772...
y.sin()           // 0.00159...
y.cos()           // -0.9999...
f64::INFINITY     // Infinity
f64::NAN          // NaN
y.is_nan()        // false
```

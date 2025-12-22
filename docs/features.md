## Table of Contents

- [ ] [Core Syntax & Basic Types](#core-syntax)
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
#### [ ]primitive types
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

#### [ ]type inference
```rust
// Type inference - compiler deduces types
let x = 42              // Inferred as i32
let y = 3.14            // Inferred as f64
let flag = true         // Inferred as bool
let text = "Hello"      // Inferred as str

// Mixed usage
let sum = x + 10        // x is i32, sum is i32
```


#### [ ]numeric literals
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

#### [ ]immutable by default
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

#### [ ]shadowing
```rust
let x = 5
let x = x + 1        // Shadow previous x
let x = x * 2        // Shadow again (x is now 12)

// Can change type with shadowing
let spaces = "   "
let spaces = spaces.len()  // Now an integer
```

#### [ ]destructing
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
#### [ ]basic functions
```rust
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

#### [ ]default argument
```rust
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


#### [ ]function overloading
```rust
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

#### [ ]higher-order function
```rust
// Function taking another function as parameter
func apply_twice(f: fn(i32) -> i32, x: i32) -> i32 {
    f(f(x))
}

func increment(x: i32) -> i32 {
    x + 1
}

let result = apply_twice(increment, 5)  // 7
```


#### [ ]closures
```rust
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
### Control Flow


#### [ ]ifExpressions
```rust
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
#### [ ]loops
```rust
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

#### [ ]continue
```rust
for i in 0..10 {
    if i % 2 == 0 {
        continue  // Skip even numbers
    }
    println("{}", i)
}
```

---

<a id="ownership-borrowing"></a>
### Ownership & Borrowing



#### [ ]ownership rules
```rust
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


#### [ ]move semantics
```rust
func take_ownership(s: String) {
    println("{}", s)
}  // s is dropped here

let s = String::from("hello")
take_ownership(s)
// println("{}", s)  // ERROR: s was moved
```

#### [ ]borrowing(reference)
```rust
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

#### [ ]borrowing rulse
```rust
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

#### [ ]reference vs raw pointer

```rust
// Safe reference (ownership checked)
let x = 5
let r = &x  // Immutable reference

// Raw pointer (unsafe)
unsafe {
    let r = &x as *const i32
    let mut_r = &mut x as *mut i32
    println!("{}", *r)  // Dereference in unsafe block
}
```

---

<a id="type-system-inference"></a>
### Type System & Inference

#### [ ]type annotations

```rust
// Explicit type annotations
let x: i32 = 5
let y: f64 = 3.14
let flag: bool = true

// Type inference
let x = 5          // i32 inferred
let y = 3.14       // f64 inferred
let flag = true    // bool inferred
```

#### [ ]compound types
```rust
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


#### [ ]type aliases
```rust
// Type alias
type Kilometers = i32
type Point = (i32, i32)

let distance: Kilometers = 100
let origin: Point = (0, 0)
```
#### [ ]never type
```rust
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
### Generics

#### [ ]generic function
```rust
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


#### [ ]generic structs
```rust
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

#### [ ]generic enums
```rust
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

#### [ ]generic methods
```rust
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

#### [ ]monomorphization
```rust
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
### Pattern Matching

#### [ ]match expression
```rust
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

#### [ ]destructuring
```rust
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

#### [ ]match guards

```rust
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

#### [ ]ifLet and whileLet
```rust
// If let (shorthand for single pattern match)
let some_value = Option::Some(5)

if let Option::Some(x) = some_value {
    println("got value: {}", x)
} else {
    println("no value")
}

// While let (loop until pattern doesn't match)
let mut stack = vec![1, 2, 3]

while let Option::Some(top) = stack.pop() {
    println("{}", top)
}
```

#### [ ]pattern matching in function parameters

```rust
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
### Error Handling


#### [ ]option<T>
```rust
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


#### [ ]result<T,E>
```rust
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

#### [ ]panic(unrecoverable errors)
```rust
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

#### [ ]custom error types
```rust
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


---

<a id="object-oriented-programming"></a>
### Object-Oriented Programming


#### [ ]structs
```
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


#### [ ]methods
```rust
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

#### [ ]classes(inheritance)

```rust
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

#### [ ]visibility

```rust
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
### Traits & Interfaces

#### [ ]defining traits

```rust
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

#### [ ]implementing traits

```rust
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
        format!("{} says goodbye", self.name)
    }
}

// Usage
let dog = Dog { name: String::from("Buddy") }
println("{}", dog.speak())        // "Woof!"
println("{}", dog.greet())        // "Hello" (default)
println("{}", dog.say_goodbye())  // "Buddy says goodbye"
```

#### [ ]trait bounds

```rust
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

#### [ ]built-in traits

```rust
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

#### [ ]trait objects(dynamic dispatch)

```rust
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

<a id="functional-programming"></a>
### Functional Programming

#### [ ]first-class functions

```rust
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

#### [ ]closures

```rust
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

#### [ ]higher-order functions

```rust

```
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

# Nova Language Specification

> TODO: Complete language specification

## 1. Lexical Structure

### 1.1 Keywords
```
let, mut, func, class, struct, enum, trait, impl,
if, else, match, for, while, loop, break, continue, return,
pub, priv, mod, use, as, in, where,
true, false, none, self, Self
```

### 1.2 Operators
- [ ] Arithmetic: `+`, `-`, `*`, `/`, `%`
- [ ] Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`
- [ ] Logical: `&&`, `||`, `!`
- [ ] Bitwise: `&`, `|`, `^`, `~`, `<<`, `>>`
- [ ] Assignment: `=`, `+=`, `-=`, `*=`, `/=`
- [ ] Reference: `&`, `&mut`

### 1.3 Literals
- [ ] Integer: `42`, `0x2A`, `0b101010`, `0o52`
- [ ] Float: `3.14`, `2.5e10`
- [ ] String: `"hello"`, `"""multiline"""`
- [ ] Char: `'a'`, `'\n'`
- [ ] Bool: `true`, `false`

## 2. Types

### 2.1 Primitive Types
```nova
i8, i16, i32, i64, i128, isize
u8, u16, u32, u64, u128, usize
f32, f64
bool
char
str
```

### 2.2 Compound Types
- [ ] Tuples: `(T1, T2, ...)`
- [ ] Arrays: `[T; N]`
- [ ] Slices: `[T]`
- [ ] References: `&T`, `&mut T`

### 2.3 User-Defined Types
- [ ] Structs
- [ ] Enums
- [ ] Classes
- [ ] Traits

## 3. Declarations

### 3.1 Variables
```nova
let x = 42;           // immutable
let mut y = 0;        // mutable
let z: i32 = 10;      // explicit type
```

### 3.2 Functions
```nova
func add(a: i32, b: i32) -> i32 {
    a + b
}
```

### 3.3 Classes
```nova
class Point {
    x: f64,
    y: f64,
    
    func new(x: f64, y: f64) -> Self {
        Point { x, y }
    }
    
    func distance(&self, other: &Point) -> f64 {
        // ...
    }
}
```

### 3.4 Traits
```nova
trait Display {
    func fmt(&self) -> String;
}
```

## 4. Expressions

TODO: Document all expression types

## 5. Statements

TODO: Document all statement types

## 6. Pattern Matching

TODO: Document pattern matching

## 7. Ownership & Borrowing

TODO: Document ownership rules

## 8. Generics

TODO: Document generic types and constraints

## 9. Modules

TODO: Document module system

## 10. Error Handling

TODO: Document Result/Option types

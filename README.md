# Nova Programming Language

**The C++ you wish existed — modern, safe, no C legacy.**

Nova is a systems programming language for developers who love C++'s power (classes, templates, RAII, operator overloading) but want to leave behind its C baggage (headers, preprocessor, null pointers, undefined behavior).

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)

## Why Nova?

### What You Keep from C++
- ✅ **Classes with inheritance** - Real OOP, not just traits
- ✅ **Templates/Generics** - Familiar `<T>` syntax
- ✅ **RAII** - Deterministic resource management
- ✅ **Operator overloading** - Natural syntax for custom types
- ✅ **Value semantics** - Efficient, predictable memory layout
- ✅ **Zero-cost abstractions** - No runtime overhead

### What You Leave Behind (C Legacy)
- ❌ **No headers** - Single-file modules with `use` imports
- ❌ **No preprocessor** - No `#include`, `#define`, `#ifdef` hell
- ❌ **No null pointers** - `Option<T>` instead
- ❌ **No undefined behavior** - Everything is defined
- ❌ **No manual memory management** - Ownership system handles it
- ❌ **No implicit conversions** - Explicit is better
- ❌ **No `void*`** - Use generics `<T>` instead

## Features

- **Memory Safety** - Ownership system with borrowing (no garbage collector)
- **Zero-Cost Abstractions** - High-level features compile to efficient machine code
- **Type System** - Strong + Static + Inference
- **RAII** - Scope-based drop, deterministic destruction
- **Functional-First** - Immutable by default, expressions everywhere, pattern matching
- **Error Handling** - `Result<T, E>` and `Option<T>` for explicit error handling
- **Modern OOP** - Classes and traits when you need them

## Quick Start

### Prerequisites

- CMake 3.15+
- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- Git

### Build

```bash
# Clone the repository
git clone https://github.com/your-username/Nova-Lang.git
cd Nova-Lang

# Build the project
./scripts/build.sh

# Run tests
./scripts/test.sh
```

### Usage

```bash
# Compile a Nova file
./build/bin/nova examples/hello_world.nova

# Start the REPL
./build/bin/nova-repl
```

## Hello World

```nova
func main() {
    println("Hello, World!")
}
```

## Example: Classes (No Headers!)

```nova
// No #include, no forward declarations, no header files
class Animal {
    priv name: String
    
    pub func new(name: String) -> Self {
        Self { name }
    }
    
    pub func speak(&self) {
        println("{} makes a sound", self.name)
    }
}

class Dog : Animal {
    pub func speak(&self) {
        println("{} barks!", self.name)
    }
}

func main() {
    let dog = Dog::new("Buddy")
    dog.speak()  // "Buddy barks!"
}
```

## Example: No Null, Use Option

```nova
func find_user(id: i32) -> Option<User> {
    if id == 1 {
        Option::Some(User::new("Alice"))
    } else {
        Option::None
    }
}

func main() {
    // No null pointer exceptions - compiler enforces handling
    match find_user(1) {
        Some(user) => println("Found: {}", user.name),
        None => println("User not found")
    }
}
```

## Example: RAII (Automatic Cleanup)

```nova
class File {
    priv handle: FileHandle
    
    pub func open(path: str) -> Result<Self, IoError> {
        // ...
    }
}

impl Drop for File {
    func drop(&mut self) {
        // Automatically called when File goes out of scope
        self.handle.close()
    }
}

func main() {
    let file = File::open("data.txt")?
    // Use file...
}   // file.drop() called automatically here - no leaks!
```

## Project Structure

```
Nova-Lang/
├── include/nova/         # Public headers
│   ├── AST/              # Abstract syntax tree
│   ├── Analysis/         # Ownership & borrow checking
│   ├── Basic/            # Core utilities (SourceManager, Diagnostics)
│   ├── CodeGen/          # Code generation
│   ├── Driver/           # Compiler driver
│   ├── IR/               # Intermediate representation
│   ├── Lex/              # Lexer and tokens
│   ├── Parse/            # Parser
│   ├── Runtime/          # Runtime support
│   ├── Sema/             # Semantic analysis
│   └── Transforms/       # Optimization passes
├── lib/                  # Implementation (.cpp files)
│   └── (mirrors include/nova structure)
├── stdlib/               # Standard library (Nova source)
│   ├── core/             # Core types & traits
│   │   ├── option.nova   #   Option<T>
│   │   ├── result.nova   #   Result<T, E>
│   │   ├── iter.nova     #   Iterator trait
│   │   ├── ops.nova      #   Operator traits
│   │   ├── cmp.nova      #   Comparison traits
│   │   ├── clone.nova    #   Clone, Copy
│   │   ├── fmt.nova      #   Debug, Display
│   │   └── ...           #   mem, ptr, slice, string
│   ├── collections/      # Data structures
│   │   ├── vec.nova      #   Vec<T>
│   │   ├── hashmap.nova  #   HashMap<K, V>
│   │   ├── hashset.nova  #   HashSet<T>
│   │   └── ...           #   deque, linkedlist
│   ├── io/               # Input/Output
│   ├── sync/             # Synchronization (Mutex, Arc)
│   ├── thread/           # Threading
│   ├── async/            # Async runtime
│   └── error.nova        # Error trait
├── tools/                # Executables
│   ├── nova/             # Compiler
│   └── nova-repl/        # Interactive REPL
├── tests/                # Test suite
├── examples/             # Example programs
├── docs/                 # Documentation
└── scripts/              # Build scripts
```

## Documentation

- [Language Features](docs/features.md) - Complete language reference
- [Implementation Guide](docs/implement.md) - Compiler implementation details
- [Implementation Plan](docs/implementation-plan.md) - Development roadmap
- [CMake Guide](docs/cmake-guide.md) - Build system documentation

## Comparison

| Feature | C++ | Nova |
|---------|-----|------|
| Headers | `.h`/`.hpp` files | None - single file modules |
| Null | `nullptr`, raw pointers | `Option<T>` |
| Memory | `new`/`delete`, smart pointers | Ownership (automatic) |
| Strings | `char*`, `std::string` | `String`, `str` |
| Errors | Exceptions, error codes | `Result<T, E>` |
| Preprocessor | `#include`, `#define` | None |
| Inheritance | `class Derived : public Base` | `class Derived : Base` |
| Generics | `template<typename T>` | `<T>` |
| UB | Everywhere | None - everything defined |

## Contributing

Contributions are welcome! Please read the documentation in `docs/` before contributing.

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

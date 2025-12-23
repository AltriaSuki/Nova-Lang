# Nova Documentation

Welcome to the Nova programming language documentation.

> **Nova: The C++ you wish existed — modern, safe, no C legacy.**

Nova is for developers who love C++'s power but want to leave behind its historical baggage. Keep the classes, templates, RAII, and operator overloading. Ditch the headers, preprocessor, null pointers, and undefined behavior.

---

## Table of Contents

- [Getting Started](#getting-started)
- [Documentation Files](#documentation-files)
- [Project Structure](#project-structure)
- [Building Nova](#building-nova)
- [Contributing](#contributing)

---

## Getting Started

### Nova's Philosophy

| Principle | Description |
|-----------|-------------|
| **Functional-First** | Immutable by default, expressions everywhere, pattern matching |
| **Strong + Static + Inference** | Types checked at compile time, but inferred when obvious |
| **RAII** | Scope-based drop - resources freed when scope ends |
| **No GC** | Ownership system, not garbage collection |
| **Zero-Cost Abstractions** | Pay only for what you use |

| Keep from C++ | Remove (C Legacy) |
|---------------|-------------------|
| Classes + inheritance | Header files |
| Templates `<T>` | Preprocessor `#include/#define` |
| RAII | `NULL` / raw pointers |
| Operator overloading | Undefined behavior |
| Value semantics | `malloc`/`free` |
| Zero-cost abstractions | Implicit conversions |

### Quick Example
```nova
// No headers, no forward declarations
class Person {
    priv name: String
    priv age: i32
    
    pub func new(name: String, age: i32) -> Self {
        Self { name, age }
    }
    
    pub func greet(&self) {
        println("Hi, I'm {self.name}")
    }
}

func main() {
    let person = Person::new("Alice", 30)
    person.greet()
}
```

---

## Documentation Files

| Document | Description |
|----------|-------------|
| [features.md](features.md) | Comprehensive guide to Nova's language features with examples |
| [implement.md](implement.md) | Short implementation sequence for quick reference |
| [implementation-plan.md](implementation-plan.md) | Detailed implementation roadmap with file-level tasks |
| [cmake-guide.md](cmake-guide.md) | CMake build system guide and best practices |

---

## Project Structure

```
Nova-Lang/
├── include/nova/         # Public headers
│   ├── AST/              # Abstract syntax tree
│   ├── Analysis/         # Ownership & borrow checking
│   ├── Basic/            # Core utilities
│   ├── CodeGen/          # Code generation
│   ├── Driver/           # Compiler driver
│   ├── IR/               # Intermediate representation
│   ├── Lex/              # Lexer and tokens
│   ├── Parse/            # Parser
│   ├── Runtime/          # Runtime support
│   ├── Sema/             # Semantic analysis
│   └── Transforms/       # Optimizations
├── lib/                  # Implementation (.cpp)
├── stdlib/               # Standard library (Nova)
│   ├── core/             # option, result, iter, ops, cmp, clone, fmt, mem, ptr, slice, string
│   ├── collections/      # vec, hashmap, hashset, deque, linkedlist
│   ├── io/               # File I/O
│   ├── sync/             # mutex, rwlock, arc
│   ├── thread/           # Threading
│   ├── async/            # Async runtime
│   └── error.nova        # Error trait
├── tools/                # nova, nova-repl
├── tests/                # Test suite
├── examples/             # Examples
├── docs/                 # Documentation
└── scripts/              # Build scripts
```

---

## Building Nova

### Prerequisites
- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- CMake 3.15+
- (Optional) LLVM 14+ for native code generation

### Build Steps
```bash
# Clone the repository
git clone https://github.com/yourusername/Nova-Lang.git
cd Nova-Lang

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)

# Run tests
make test
```

### Build Scripts
```bash
# Or use the provided scripts
./scripts/build.sh    # Build the compiler
./scripts/test.sh     # Run the test suite
```

---

## Contributing

Nova is an open-source project and welcomes contributions!

### Development Workflow
1. Check [implementation-plan.md](implementation-plan.md) for current tasks
2. Pick an unchecked `[ ]` item to work on
3. Create a feature branch
4. Implement with tests
5. Submit a pull request

### Coding Standards
- Follow existing code style
- Add unit tests for new features
- Update documentation as needed
- Keep commits focused and well-documented

---

## Current Status

Nova is under active development. See the implementation tracking documents for current progress:

- **Phase 1**: Core Infrastructure (Files created)
- **Phase 2**: Lexer (Files created)
- **Phase 3**: Parser & AST (In Progress)
- **Phase 4**: Semantic Analysis (In Progress)
- **Phase 5+**: Advanced Features (Planned)

Check [implement.md](implement.md) to track implementation tasks and [features.md](features.md) for language feature documentation.

---

## Resources

- [Language Features](features.md) - Complete feature reference
- [Implementation Tasks](implement.md) - Quick task checklist
- [Implementation Plan](implementation-plan.md) - Detailed development roadmap
- [Examples](../examples/) - Sample Nova programs

---

## License

Nova is released under the MIT License. See [LICENSE](../LICENSE) for details.

# Nova Programming Language (Nova-Lang)

Nova is a learning-oriented compiler project for a future systems language that aims to combine a small, well-defined core with an optional LLVM backend.

## Status

This repository is under active development and is not yet a working Nova compiler. Most subsystems are scaffolding (headers, CMake targets, and placeholders) intended to support incremental implementation.

See `docs/status.md` for a summary of the current implementation status.

**Implemented (usable today):**
- `nova::SourceLocation` / `nova::SourceRange` and basic source file management in `nova::SourceManager`.
- Token kind definitions (`TokenKind`) and token spelling helpers.

**Scaffolded (APIs exist, implementation incomplete):**
- Diagnostics (`DiagnosticEngine`), lexer (`Lexer`), parser/AST, semantic analysis, IR, interpreter, and LLVM code generation.

## Getting Started

### Prerequisites

- CMake 3.15+
- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)

### Build

```bash
./scripts/build.sh
```

Outputs are placed in `build/bin/`.

### Run

```bash
./build/bin/nova
./build/bin/nova-repl
```

## Language Design (Draft)

The `.nova` examples in this repo illustrate the *intended* surface syntax. The authoritative draft spec lives in `docs/language-spec.md`, and the aspirational feature catalog is in `docs/features.md`.

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

- `docs/README.md` — documentation index and where to start
- `docs/language-spec.md` — draft language spec (small, testable core)
- `docs/architecture.md` — compiler architecture (modules and data flow)
- `docs/developer-guide.md` — build, tests, workflow
- `docs/implementation-plan.md` / `docs/implement.md` — roadmap and task lists
- `docs/cmake-guide.md` — build system notes (LLVM + tests)

## Contributing

Contributions are welcome. Start from `docs/developer-guide.md` and pick a small, isolated task from `docs/implement.md`.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

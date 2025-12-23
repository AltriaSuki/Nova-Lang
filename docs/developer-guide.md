# Nova Developer Guide

> TODO: Complete developer guide

## Getting Started

### Prerequisites
- CMake 3.15+
- C++20 compatible compiler (GCC 10+, Clang 12+, MSVC 2019+)
- LLVM 15+ (optional, for native code generation)

### Building
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Running Tests
```bash
cd build
ctest --output-on-failure
```

## Project Structure

```
Nova-Lang/
├── include/nova/     # Public headers
├── lib/              # Implementation
├── tools/            # Executables (nova, nova-repl)
├── stdlib/           # Standard library
├── tests/            # Test suite
└── docs/             # Documentation
```

## Contributing

### Code Style
- [ ] Use clang-format with project .clang-format
- [ ] Follow naming conventions:
  - Classes: `PascalCase`
  - Functions: `camelCase`
  - Variables: `camelCase`
  - Constants: `UPPER_SNAKE_CASE`
  - Files: `PascalCase.cpp/.hpp`

### Adding a New Feature

1. Design phase
   - [ ] Write design doc
   - [ ] Get approval

2. Implementation
   - [ ] Add AST nodes if needed
   - [ ] Update parser
   - [ ] Add semantic checks
   - [ ] Add IR support
   - [ ] Add codegen

3. Testing
   - [ ] Unit tests
   - [ ] Integration tests
   - [ ] Documentation

### Pull Request Process
- [ ] All tests pass
- [ ] Code review approved
- [ ] Documentation updated

## Debugging

### Compiler Flags
```bash
nova --dump-tokens file.nova    # Dump lexer output
nova --dump-ast file.nova       # Dump AST
nova --dump-ir file.nova        # Dump IR
```

### GDB/LLDB Tips
TODO: Add debugging tips

## Architecture Deep Dive

See [architecture.md](architecture.md) for detailed architecture documentation.

## API Reference

See [api/](api/) directory for API documentation.

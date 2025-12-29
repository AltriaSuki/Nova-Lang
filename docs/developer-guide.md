# Nova Developer Guide

This document describes build and development procedures for the Nova-Lang codebase. Nova-Lang is currently a scaffolded compiler project; many subsystems are intentionally incomplete.

## Prerequisites

- CMake 3.15+
- C++20 compiler (GCC 10+, Clang 12+, MSVC 2019+)
- (Optional) LLVM (for the optional LLVM backend target; see `docs/cmake-guide.md`)

## C++ Standard Policy

- The project targets **C++20** (`CMAKE_CXX_STANDARD=20`) and should remain buildable as C++20.
- Avoid mixing language standards across targets (e.g. some libraries in C++20, others in C++23) unless there is a strong reason and it is documented.
- Newer compiler toolchains may be used; however, C++23/C++26-only language or library features should not be introduced unless the required standard is explicitly raised.

## Build

### Using the scripts

```bash
./scripts/build.sh
```

This creates `build/` and places executables in `build/bin/`.

### Manual CMake (recommended for development use)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

### Enabling the LLVM backend (optional)

The LLVM backend library target (`novaLLVMCodeGen`) is built only if CMake can find LLVM via `find_package(LLVM CONFIG)` and LLVM is enabled.

If LLVM is installed but not discovered automatically, set `LLVM_DIR`:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DLLVM_DIR=/path/to/lib/cmake/llvm
cmake --build build -j
```

To force-disable LLVM even if it is installed:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DNOVA_ENABLE_LLVM=OFF
cmake --build build -j
```

## Tests

Nova uses GoogleTest for unit tests.

Notes:
- If `GTest` is not installed, CMake will attempt to download it via `FetchContent` (`tests/CMakeLists.txt`). In restricted or offline environments, it may be necessary to install GTest via the system package manager or to disable tests.

Build without tests:

```bash
cmake -S . -B build -DNOVA_BUILD_TESTS=OFF
cmake --build build -j
```

Run tests (when enabled):

```bash
cd build
ctest --output-on-failure
```

## Formatting

When `clang-format` is available:

```bash
clang-format -i tools/nova/nova.cpp
```

A repository-wide formatting script is not currently provided; contributions should maintain minimal and reviewable changes.

## Running Tools

```bash
./build/bin/nova
./build/bin/nova-repl
```

At present these binaries are placeholders (they print version text and do not compile or execute Nova code). As compiler functionality is implemented, this document will be updated to reflect supported CLI options.

## Working Offline

By default, the test suite may attempt to download GoogleTest via CMake `FetchContent` if `GTest` is not installed.

Possible approaches include:
- installing GoogleTest system-wide, or
- building with tests disabled (`-DNOVA_BUILD_TESTS=OFF`).

## Code Style

- Formatting: `clang-format` with the repository `.clang-format`.
- Prefer small, reviewable changes and update docs alongside behavior.

## Adding New Code

Recommended workflow:

1. Add/extend headers in `include/nova/<Module>/`.
2. Add/extend implementations in `lib/<Module>/`.
3. Update the corresponding `lib/<Module>/CMakeLists.txt`.
4. Add/extend unit tests in `tests/unit/`.
5. Update `docs/language-spec.md` if the language contract changes.

## Adding a New Compiler Flag (Suggested Pattern)

As the `nova` CLI grows, prefer implementing flags in the `Driver` layer and keeping subsystems reusable as libraries.

Common debug-dump flags intended for early implementation:
- `--dump-tokens`
- `--dump-ast`
- `--dump-ir`

Each flag should have:
- deterministic output (stable across runs)
- unit tests or golden tests

## Debugging Tips

Build a Debug configuration and use an appropriate debugger:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
lldb ./build/bin/nova
```

The project enables `compile_commands.json` generation via `CMAKE_EXPORT_COMPILE_COMMANDS`.

## Further Reading

- `docs/architecture.md` — module boundaries and data flow
- `docs/api/README.md` — public headers and API documentation approach

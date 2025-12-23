# CMake Guide for Nova-Lang

A comprehensive guide to understanding and maintaining the CMake build system for the Nova compiler.

---

## Table of Contents

- [Project Structure Analysis](#project-structure-analysis)
- [Current Issues Found](#current-issues-found)
- [CMake Best Practices for Large Projects](#cmake-best-practices-for-large-projects)
- [Nova CMake Architecture](#nova-cmake-architecture)
- [Adding New Components](#adding-new-components)
- [Common Patterns](#common-patterns)
- [Troubleshooting](#troubleshooting)

---

## Project Structure Analysis

### Current CMakeLists.txt Hierarchy

```
Nova-Lang/
├── CMakeLists.txt              # Root: project config, options, subdirs
├── lib/
│   ├── CMakeLists.txt          # Aggregates all library subdirs
│   ├── Basic/CMakeLists.txt    # novaBasic library
│   ├── Lex/CMakeLists.txt      # novaLex library
│   ├── Parse/CMakeLists.txt    # novaParse library
│   ├── AST/CMakeLists.txt      # novaAST library
│   ├── Sema/CMakeLists.txt     # novaSema library
│   ├── CodeGen/
│   │   ├── CMakeLists.txt      # novaCodeGen library
│   │   └── Interpreter/
│   │       └── CMakeLists.txt  # novaInterpreter library
│   ├── Driver/CMakeLists.txt   # novaDriver library
│   ├── IR/CMakeLists.txt       # novaIR library
│   ├── Transforms/CMakeLists.txt # novaTransforms library
│   ├── Runtime/CMakeLists.txt  # novaRuntime library
│   └── Analysis/CMakeLists.txt # novaAnalysis library
├── tools/
│   ├── CMakeLists.txt          # Aggregates tool subdirs
│   ├── nova/CMakeLists.txt     # nova executable
│   └── nova-repl/CMakeLists.txt # nova-repl executable
└── tests/
    ├── CMakeLists.txt          # Test config, GTest fetch
    └── unit/CMakeLists.txt     # Unit test executable
```

### Library Dependency Graph

```
novaBasic (foundation - no dependencies)
    │
    ├── novaLex ─────────────────┐
    │                            │
    ├── novaAST ─────────────────┼──► novaParse
    │       │                    │
    │       ├── novaSema ────────┘
    │       │       │
    │       │       └── novaAnalysis
    │       │
    │       ├── novaCodeGen
    │       │
    │       ├── novaIR ──► novaTransforms
    │       │
    │       └── novaInterpreter ◄── novaRuntime
    │
    └── novaDriver
```

---

## Current Issues Found

### ❌ Issue 1: Interpreter Subdirectory Not Included

**Location:** `lib/CodeGen/CMakeLists.txt`

**Problem:** The `Interpreter/` subdirectory is not added via `add_subdirectory()`, so `novaInterpreter` is never built.

**Fix:**
```cmake
# lib/CodeGen/CMakeLists.txt
add_library(novaCodeGen
    CodeGenModule.cpp
)

target_link_libraries(novaCodeGen PUBLIC
    novaBasic
    novaAST
)

target_include_directories(novaCodeGen PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# ADD THIS LINE:
add_subdirectory(Interpreter)
```

### ⚠️ Issue 2: Missing Libraries in Tool Linking

**Location:** `tools/nova/CMakeLists.txt`

**Problem:** The `nova` executable doesn't link against some libraries it may need:
- `novaInterpreter` - for running interpreted code
- `novaIR` - for IR generation
- `novaTransforms` - for optimizations
- `novaAnalysis` - for borrow checking
- `novaRuntime` - for built-in functions

**Current:**
```cmake
target_link_libraries(nova PRIVATE
    novaDriver
    novaCodeGen
    novaSema
    novaParse
    novaLex
    novaAST
    novaBasic
)
```

**Suggested Fix:**
```cmake
target_link_libraries(nova PRIVATE
    novaDriver
    novaCodeGen
    novaInterpreter  # ADD
    novaSema
    novaParse
    novaLex
    novaAST
    novaBasic
    novaIR           # ADD
    novaTransforms   # ADD
    novaAnalysis     # ADD
    novaRuntime      # ADD
)
```

### ⚠️ Issue 3: Redundant Include Directories

**Problem:** Every library sets `target_include_directories()` to the same path. This is redundant because the root `CMakeLists.txt` already has `include_directories(${PROJECT_SOURCE_DIR}/include)`.

**Options:**
1. Remove `target_include_directories()` from all library CMakeLists (simpler)
2. Remove global `include_directories()` and keep per-target (more explicit, modern CMake)

**Recommended:** Option 2 (modern CMake approach) - already being done, just remove the global one from root.

### ⚠️ Issue 4: Missing Output Directory Configuration

**Problem:** Executables go to `build/tools/nova/` instead of a unified `build/bin/`.

**Fix in root CMakeLists.txt:**
```cmake
# Add after project() declaration
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
```

### ✅ Good Practices Already in Use

1. ✅ CMake 3.15 minimum version
2. ✅ C++20 standard enforced
3. ✅ Compile commands exported (`CMAKE_EXPORT_COMPILE_COMMANDS`)
4. ✅ Platform-specific compiler warnings
5. ✅ Option for building tests
6. ✅ FetchContent for GoogleTest
7. ✅ `target_link_libraries` with PUBLIC/PRIVATE visibility
8. ✅ Proper install target for nova executable

---

## CMake Best Practices for Large Projects

### 1. Project Organization

```cmake
# Root CMakeLists.txt structure
cmake_minimum_required(VERSION 3.15)
project(MyProject VERSION 1.0.0 LANGUAGES CXX)

# Global settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler extensions

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_DOCS "Build documentation" OFF)

# Subdirectories
add_subdirectory(lib)
add_subdirectory(tools)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
```

### 2. Modern Target-Based Approach

**❌ Old style (avoid):**
```cmake
include_directories(${PROJECT_SOURCE_DIR}/include)
link_libraries(somelib)
```

**✅ Modern style (use):**
```cmake
target_include_directories(mylib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(mylib PUBLIC dependency)
```

### 3. Visibility Keywords

| Keyword | Meaning |
|---------|---------|
| `PUBLIC` | Used by this target AND propagates to consumers |
| `PRIVATE` | Used only by this target |
| `INTERFACE` | Not used by this target, only propagates to consumers |

**Example:**
```cmake
# novaLex needs novaBasic headers (PUBLIC)
# novaLex uses internal impl (PRIVATE)
target_link_libraries(novaLex
    PUBLIC novaBasic      # Consumers of novaLex also need novaBasic
    PRIVATE internalLib   # Only novaLex needs this
)
```

### 4. Library Definition Pattern

```cmake
# lib/MyModule/CMakeLists.txt
add_library(myModule
    Source1.cpp
    Source2.cpp
)

# Prefer using an alias for consistency
add_library(Nova::MyModule ALIAS myModule)

target_include_directories(myModule
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(myModule
    PUBLIC Nova::Basic
)

# Set properties
set_target_properties(myModule PROPERTIES
    OUTPUT_NAME nova_mymodule
    VERSION ${PROJECT_VERSION}
)
```

### 5. Executable Pattern

```cmake
add_executable(myTool
    main.cpp
)

target_link_libraries(myTool PRIVATE
    Nova::Driver
    Nova::CodeGen
)

install(TARGETS myTool
    RUNTIME DESTINATION bin
)
```

### 6. Finding and Using Dependencies

```cmake
# For system-installed packages
find_package(LLVM 14 REQUIRED CONFIG)
target_link_libraries(myTarget PRIVATE LLVM::LLVM)

# For header-only libraries
find_package(nlohmann_json 3.11 REQUIRED)
target_link_libraries(myTarget PRIVATE nlohmann_json::nlohmann_json)

# For fetching dependencies
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)
```

### 7. Generator Expressions

```cmake
# Different flags for Debug/Release
target_compile_options(myTarget PRIVATE
    $<$<CONFIG:Debug>:-O0 -g>
    $<$<CONFIG:Release>:-O3>
)

# Platform-specific
target_compile_definitions(myTarget PRIVATE
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
)

# Build vs Install paths
target_include_directories(myTarget PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

### 8. Testing with CTest

```cmake
# tests/CMakeLists.txt
include(CTest)

add_executable(unit_tests
    test_main.cpp
    test_lexer.cpp
)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    Nova::Lex
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
```

---

## Nova CMake Architecture

### Recommended Improved Structure

```cmake
# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(Nova VERSION 0.1.0 LANGUAGES CXX)

# Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler warnings
add_library(nova_compiler_flags INTERFACE)
target_compile_options(nova_compiler_flags INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# Options
option(NOVA_BUILD_TESTS "Build tests" ON)
option(NOVA_ENABLE_LLVM "Enable LLVM backend" OFF)

# Subdirectories
add_subdirectory(lib)
add_subdirectory(tools)

if(NOVA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Summary
message(STATUS "")
message(STATUS "Nova Language Compiler v${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build Tests: ${NOVA_BUILD_TESTS}")
message(STATUS "  LLVM Backend: ${NOVA_ENABLE_LLVM}")
message(STATUS "")
```

---

## Adding New Components

### Adding a New Library

1. Create directory: `lib/NewModule/`
2. Create `lib/NewModule/CMakeLists.txt`:

```cmake
add_library(novaNewModule
    File1.cpp
    File2.cpp
)

target_link_libraries(novaNewModule PUBLIC
    novaBasic
    # other dependencies
)

target_include_directories(novaNewModule PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
```

3. Add to `lib/CMakeLists.txt`:
```cmake
add_subdirectory(NewModule)
```

4. Create headers: `include/nova/NewModule/`

### Adding a New Tool

1. Create directory: `tools/new-tool/`
2. Create `tools/new-tool/CMakeLists.txt`:

```cmake
add_executable(nova-new-tool
    main.cpp
)

target_link_libraries(nova-new-tool PRIVATE
    novaBasic
    novaLex
    # etc
)

install(TARGETS nova-new-tool RUNTIME DESTINATION bin)
```

3. Add to `tools/CMakeLists.txt`:
```cmake
add_subdirectory(new-tool)
```

### Adding New Tests

1. Add test file to `tests/unit/NewTest.cpp`
2. Update `tests/unit/CMakeLists.txt`:

```cmake
add_executable(novaTests
    LexerTest.cpp
    SourceLocationTest.cpp
    NewTest.cpp  # ADD
)
```

---

## Common Patterns

### Conditional Compilation

```cmake
# Optional LLVM support
option(NOVA_ENABLE_LLVM "Enable LLVM backend" OFF)

if(NOVA_ENABLE_LLVM)
    find_package(LLVM REQUIRED CONFIG)
    
    target_sources(novaCodeGen PRIVATE
        LLVMCodeGen.cpp
    )
    
    target_link_libraries(novaCodeGen PRIVATE
        LLVM::Core
        LLVM::Support
    )
    
    target_compile_definitions(novaCodeGen PUBLIC
        NOVA_ENABLE_LLVM=1
    )
endif()
```

### Interface Library for Headers

```cmake
# For header-only components
add_library(novaHeaders INTERFACE)
target_include_directories(novaHeaders INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)
```

### Collecting Sources with GLOB (use carefully)

```cmake
# Not recommended for source files, but useful for headers
file(GLOB_RECURSE HEADERS
    ${PROJECT_SOURCE_DIR}/include/nova/*.hpp
)

# Better: explicitly list files
set(NOVA_BASIC_SOURCES
    SourceLocation.cpp
    SourceManager.cpp
    Diagnostic.cpp
)
```

---

## Troubleshooting

### Common Errors

| Error | Cause | Solution |
|-------|-------|----------|
| "Target not found" | Library not in `add_subdirectory` | Add missing `add_subdirectory()` |
| "Undefined reference" | Missing `target_link_libraries` | Add library to link list |
| "Cannot find include" | Wrong include path | Check `target_include_directories` |
| "Multiple definitions" | Source in multiple targets | Each .cpp should be in ONE target |

### Debugging CMake

```bash
# Verbose output
cmake -B build --trace-source=CMakeLists.txt

# Print all variables
cmake -B build -LA

# Print specific variable
cmake -B build && cmake -L build | grep NOVA

# Clean rebuild
rm -rf build && cmake -B build && cmake --build build
```

### Useful Commands

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build -j$(nproc)

# Build specific target
cmake --build build --target nova

# Run tests
cd build && ctest --output-on-failure

# Install
cmake --install build --prefix /usr/local
```

---

## Summary of Fixes Needed

1. **Critical:** Add `add_subdirectory(Interpreter)` to `lib/CodeGen/CMakeLists.txt`
2. **Recommended:** Add missing libraries to `tools/nova/CMakeLists.txt` linking
3. **Recommended:** Add output directory configuration to root CMakeLists.txt
4. **Optional:** Remove global `include_directories()` (already covered by per-target)
5. **Optional:** Add library aliases (e.g., `Nova::Basic`)

Apply these fixes to ensure all components build correctly!

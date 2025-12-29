# Integration Tests

**Status:** Not yet implemented.

## Purpose
This directory is reserved for tests that validate interactions between multiple compiler components.

## Test Categories
- [ ] Lexer + Parser integration
- [ ] Parser + Sema integration
- [ ] Full pipeline tests
- [ ] Error recovery tests

## Running
```bash
cd build
ctest -R integration
```

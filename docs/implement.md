# Nova Implementation Sequence

## Phase 1: Core Infrastructure (Week 1-2)

### Day 1-2: Basic/SourceLocation
**File:** `lib/Basic/SourceLocation.cpp`
**Goal:** Encode source locations efficiently
**Tests:** Unit tests for location encoding

### Day 3-4: Basic/SourceManager
**File:** `lib/Basic/SourceManager.cpp`
**Goal:** Manage source files and locations
**Tests:** File loading, line/column calculation

### Day 5-6: Basic/IdentifierTable
**File:** `lib/Basic/IdentifierTable.cpp`
**Goal:** Intern strings and keywords
**Tests:** Keyword lookup, identifier interning

### Day 7: Lex/TokenKinds
**File:** `lib/Lex/TokenKinds.cpp`
**Goal:** Define all token types
**Tests:** Token name conversion

## Phase 2: Lexer (Week 3-4)

### Day 8-10: Lex/Lexer (Basic)
**Files:** `lib/Lex/Lexer.cpp`
**Goal:** Tokenize keywords, identifiers, operators
**Tests:** Basic tokenization tests

### Day 11-12: Lex/Lexer (Literals)
**Goal:** Handle numbers, strings, chars
**Tests:** Literal parsing tests

### Day 13-14: Lex/Lexer (Comments)
**Goal:** Skip line and block comments
**Tests:** Comment handling tests

## Phase 3: Parser (Week 5-8)

### Week 5: Parser Infrastructure
- AST node base classes
- Parser error recovery
- Basic expression parsing

### Week 6: Statement Parsing
- Variable declarations
- Function declarations
- Control flow statements

### Week 7: Type Parsing
- Type annotations
- Generic types
- References

### Week 8: Integration
- Connect lexer → parser
- End-to-end compilation tests

## Phase 4: Semantic Analysis (Week 9-12)

### Week 9: Type Checking
- Basic type checking
- Type inference setup

### Week 10: Type Inference
- Hindley-Milner algorithm
- Unification

### Week 11: Ownership
- Borrow checker foundation
- Lifetime tracking

### Week 12: Integration & Testing

## Phase 5: Code Generation (Week 13-16)

### Week 13: Interpreter
- Tree-walking interpreter
- Value representation

### Week 14: Standard Library
- Core types (Option, Result)
- Collections

### Week 15-16: LLVM Backend (Optional)
- IR generation
- JIT compilation
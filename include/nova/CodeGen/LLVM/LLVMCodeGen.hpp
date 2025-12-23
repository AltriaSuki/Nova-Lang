#pragma once

// TODO: Implement LLVM code generation backend

namespace llvm {
class LLVMContext;
class Module;
class IRBuilder;
class Function;
class Value;
class Type;
}

namespace nova {

class DiagnosticEngine;

namespace ast {
class Decl;
class Stmt;
class Expr;
class FuncDecl;
class Type;
}

namespace codegen {

class LLVMCodeGen;
class LLVMTypeConverter;
class LLVMExprEmitter;

} // namespace codegen
} // namespace nova

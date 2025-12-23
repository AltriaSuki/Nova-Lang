#pragma once

// TODO: Implement LLVM expression emission

namespace llvm {
class Value;
class IRBuilder;
}

namespace nova {

namespace ast {
class Expr;
}

namespace codegen {

class LLVMExprEmitter;

} // namespace codegen
} // namespace nova

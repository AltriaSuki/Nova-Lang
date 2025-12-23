#pragma once

// TODO: Implement Nova type to LLVM type conversion

namespace llvm {
class LLVMContext;
class Type;
}

namespace nova {

namespace ast {
class Type;
}

namespace codegen {

class LLVMTypeConverter;

} // namespace codegen
} // namespace nova

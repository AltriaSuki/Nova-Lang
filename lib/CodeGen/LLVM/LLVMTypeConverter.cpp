// Nova LLVM Backend - Type Converter Implementation
// TODO: Implement Nova to LLVM type mapping
//
// Type mappings:
// - i8/i16/i32/i64 -> llvm::IntegerType
// - f32/f64 -> llvm::Type::getFloatTy/getDoubleTy
// - bool -> i1
// - str -> struct { i8*, i64 }
// - Array/Slice -> struct { T*, i64 }
// - Structs -> llvm::StructType
// - Enums -> tagged union

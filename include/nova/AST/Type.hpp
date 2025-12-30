#pragma once

// TODO: Implement type AST nodes

namespace nova {
namespace ast {
//virtual base class for all types
class Type {
    public:
    virtual ~Type() = default;
};

class BuiltinType : public Type {
    enum class Kind {
        Char,
        Bool,
        I8,I16,I32,I64,
        U8,U16,U32,U64,
        F32,F64,
        Unit,
        Never,
        //add more builtin types if needed
    };
    Kind kind_;
public:
    BuiltinType(Kind k) : kind_(k) {}

    Kind get_kind() const { return kind_; }
};

class StrType : public Type {
    //string slice type &str
};

class StringType : public Type {
    //owned string type String

};

class PointerType : public Type {
    //pointer type *const T, *mut T
};

class ReferenceType : public Type {
    //reference type &T, &mut T
};

class ArrayType : public Type {
    //array type [T; N]
};

class SliceType : public Type {
    //slice type [T]
};

class TupleType : public Type {
    //tuple type (T1, T2, ...)
};

class FunctionType : public Type {
    //function type fn(T1, T2, ...) -> R
};

class StructType : public Type {
    //struct type
};

class EnumType : public Type {
    //enum type
};

class ClassType : public Type {
    //class type
};

class TraitType : public Type {
    //trait type
};

class GenericType : public Type {
    //generic type parameter
};

} // namespace ast
} // namespace nova

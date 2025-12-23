#pragma once

// TODO: Implement symbol table

#include "nova/Basic/SourceLocation.hpp"

namespace nova {
namespace ast {
class Decl;
class Type;
}

namespace sema {

class Symbol;
class VarSymbol;
class FuncSymbol;
class TypeSymbol;
class TraitSymbol;

} // namespace sema
} // namespace nova

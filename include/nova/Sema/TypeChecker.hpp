#pragma once

// TODO: Implement type checking

namespace nova {
namespace ast {
class Expr;
class Stmt;
class Decl;
class Type;
}

class DiagnosticEngine;

namespace sema {

class Scope;
class TypeChecker;

} // namespace sema
} // namespace nova

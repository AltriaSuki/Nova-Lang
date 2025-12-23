#pragma once

// TODO: Implement tree-walking interpreter

namespace nova {

class DiagnosticEngine;

namespace ast {
class Decl;
class Stmt;
class Expr;
class FuncDecl;
}

namespace interpreter {

class Value;
class Environment;
class CallStack;
class Interpreter;

} // namespace interpreter
} // namespace nova

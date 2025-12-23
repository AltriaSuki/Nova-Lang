#pragma once

// TODO: Implement Control Flow Graph

namespace nova {
namespace ast {
class Stmt;
class Expr;
class FuncDecl;
}

namespace analysis {

class CFGBlock;
class CFG;
class CFGBuilder;

} // namespace analysis
} // namespace nova

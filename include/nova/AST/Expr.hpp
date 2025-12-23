#pragma once

// TODO: Implement expression AST nodes

#include "nova/Basic/SourceLocation.hpp"

namespace nova {
namespace ast {

class Expr;
class LiteralExpr;
class IdentifierExpr;
class BinaryExpr;
class UnaryExpr;
class CallExpr;
class MethodCallExpr;
class MemberExpr;
class IndexExpr;
class CastExpr;
class IfExpr;
class MatchExpr;
class BlockExpr;
class LambdaExpr;
class TupleExpr;
class ArrayExpr;
class StructExpr;
class RangeExpr;
class AssignExpr;

} // namespace ast
} // namespace nova

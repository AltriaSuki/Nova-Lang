#pragma once

// TODO: Implement pattern AST nodes for pattern matching

#include "nova/Basic/SourceLocation.hpp"

namespace nova {
namespace ast {

class Pattern;
class WildcardPattern;
class IdentifierPattern;
class LiteralPattern;
class TuplePattern;
class StructPattern;
class EnumPattern;
class RangePattern;
class OrPattern;
class GuardedPattern;

} // namespace ast
} // namespace nova

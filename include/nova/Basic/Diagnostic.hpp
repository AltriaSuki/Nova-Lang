#pragma once
#include <cstdint>

namespace nova {

/// Severity levels for diagnostics
enum class DiagnosticSeverity : uint8_t {
    Note,       // Additional context
    Warning,    // Potential issue
    Error,      // Compilation error
    Fatal       // Unrecoverable error
};

/// Diagnostic IDs - grouped by category
enum class DiagnosticID : uint16_t {
    // Lexer errors (1xx)
    err_invalid_character = 100,
    err_unterminated_string,
    err_unterminated_char,
    err_invalid_escape_sequence,
    err_empty_char_literal,
    err_invalid_number_literal,

    // Parser errors (2xx)
    err_expected_token = 200,
    err_expected_expression,
    err_expected_type,
    err_expected_identifier,
    err_expected_semicolon,
    err_expected_closing_paren,
    err_expected_closing_brace,
    err_expected_closing_bracket,
    err_unexpected_token,
    err_invalid_declaration,

    // Type errors (3xx)
    err_type_mismatch = 300,
    err_unknown_type,
    err_cannot_infer_type,
    err_incompatible_types,
    err_invalid_cast,
    err_no_implicit_conversion,

    // Semantic errors (4xx)
    err_undefined_variable = 400,
    err_undefined_function,
    err_undefined_type,
    err_redefinition,
    err_wrong_argument_count,
    err_wrong_argument_type,
    err_not_callable,
    err_not_indexable,
    err_invalid_operand,
    err_missing_return,
    err_unreachable_code,

    // Ownership/Borrow errors (5xx)
    err_use_after_move = 500,
    err_double_move,
    err_borrow_of_moved_value,
    err_cannot_borrow_as_mutable,
    err_cannot_move_borrowed,
    err_mutable_borrow_conflict,
    err_dangling_reference,
    err_lifetime_mismatch,
    err_assign_to_immutable,

    // Warnings (9xx)
    warn_unused_variable = 900,
    warn_unused_function,
    warn_unused_import,
    warn_unreachable_code,
    warn_shadowing_variable,
    warn_implicit_conversion,
    warn_deprecated,

    // Notes (informational)
    note_declared_here = 950,
    note_previous_borrow_here,
    note_moved_here,
    note_consider_borrowing,
};

/// Get the default severity for a diagnostic ID
DiagnosticSeverity get_default_severity(DiagnosticID id);

/// Get the format string for a diagnostic ID
const char* get_diagnostic_format(DiagnosticID id);

} // namespace nova

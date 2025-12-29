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
#define NOVA_DIAGNOSTIC(name, default_severity, code, format_string) \
    name,
#include "nova/Basic/Diagnostics.def"
#undef NOVA_DIAGNOSTIC
    count,
};

/// Get the default severity for a diagnostic ID
DiagnosticSeverity get_default_severity(DiagnosticID id);

/// Get the stable numeric portion of the diagnostic code (e.g. 7 for "E0007").
uint16_t get_diagnostic_numeric_id(DiagnosticID id);

/// Get the stable diagnostic code string (e.g. "E0007")
const char* get_diagnostic_code(DiagnosticID id);

/// Get the format string for a diagnostic ID
const char* get_diagnostic_format(DiagnosticID id);

} // namespace nova

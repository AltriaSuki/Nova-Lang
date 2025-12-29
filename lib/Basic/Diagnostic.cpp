// TODO: Implement diagnostic format strings and severity mapping
#include "nova/Basic/Diagnostic.hpp"

namespace nova {
    // Anonymous namespace for internal usage
namespace {

constexpr uint16_t parse_diagnostic_code_number(const char* code) {
    if (!code) {
        return 0;
    }
    uint16_t value = 0;
    for (int i = 1; i < 5; ++i) {
        const char c = code[i];
        if (c < '0' || c > '9') {
            return 0;
        }
        value = static_cast<uint16_t>(value * 10 + static_cast<uint16_t>(c - '0'));
    }
    return value;
}

static constexpr DiagnosticSeverity kDefaultSeverities[] = {
#define NOVA_DIAGNOSTIC(name, default_severity, code, format_string) \
    DiagnosticSeverity::default_severity,
#include "nova/Basic/Diagnostics.def"
#undef NOVA_DIAGNOSTIC
};

static constexpr const char* kFormatStrings[] = {
#define NOVA_DIAGNOSTIC(name, default_severity, code, format_string) \
    format_string,
#include "nova/Basic/Diagnostics.def"
#undef NOVA_DIAGNOSTIC
};

static constexpr const char* kDiagnosticCodes[] = {
#define NOVA_DIAGNOSTIC(name, default_severity, code, format_string) \
    code,
#include "nova/Basic/Diagnostics.def"
#undef NOVA_DIAGNOSTIC
};

static constexpr uint16_t kDiagnosticCodeNumbers[] = {
#define NOVA_DIAGNOSTIC(name, default_severity, code, format_string) \
    parse_diagnostic_code_number(code),
#include "nova/Basic/Diagnostics.def"
#undef NOVA_DIAGNOSTIC
};

static constexpr unsigned kNumDiagnostics =
    static_cast<unsigned>(sizeof(kDefaultSeverities) / sizeof(kDefaultSeverities[0]));

static_assert(kNumDiagnostics ==
                  static_cast<unsigned>(DiagnosticID::count),
              "diagnostic table size must match DiagnosticID::count");
static_assert(sizeof(kDefaultSeverities) / sizeof(kDefaultSeverities[0]) ==
                  sizeof(kFormatStrings) / sizeof(kFormatStrings[0]),
              "diagnostic tables must stay in sync");
static_assert(sizeof(kDefaultSeverities) / sizeof(kDefaultSeverities[0]) ==
                  sizeof(kDiagnosticCodes) / sizeof(kDiagnosticCodes[0]),
              "diagnostic tables must stay in sync");
static_assert(sizeof(kDefaultSeverities) / sizeof(kDefaultSeverities[0]) ==
                  sizeof(kDiagnosticCodeNumbers) / sizeof(kDiagnosticCodeNumbers[0]),
              "diagnostic tables must stay in sync");

} // namespace

DiagnosticSeverity get_default_severity(DiagnosticID id) {
    auto index = static_cast<unsigned>(id);
    if (index >= kNumDiagnostics) {
        return DiagnosticSeverity::Fatal;
    }
    return kDefaultSeverities[index];
}

const char* get_diagnostic_format(DiagnosticID id) {
    auto index = static_cast<unsigned>(id);
    if (index >= kNumDiagnostics) {
        return "<unknown diagnostic>";
    }
    return kFormatStrings[index];
}

const char* get_diagnostic_code(DiagnosticID id) {
    auto index = static_cast<unsigned>(id);
    if (index >= kNumDiagnostics) {
        return "E????";
    }
    return kDiagnosticCodes[index];
}
// Get numeric ID for diagnostic
uint16_t get_diagnostic_numeric_id(DiagnosticID id) {
    auto index = static_cast<unsigned>(id);
    if (index >= kNumDiagnostics) {
        return 0;
    }
    return kDiagnosticCodeNumbers[index];
}
}

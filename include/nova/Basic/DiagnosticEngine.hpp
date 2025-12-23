#pragma once
#include "Diagnostic.hpp"
#include "SourceLocation.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <memory>

namespace nova {

class SourceManager;

/// A single diagnostic message with location and arguments
struct DiagnosticMessage {
    DiagnosticID id;
    DiagnosticSeverity severity;
    SourceLocation location;
    std::string message;
    std::vector<SourceRange> ranges;  // Additional source ranges to highlight
};

/// Builder for constructing diagnostics with arguments
class DiagnosticBuilder {
private:
    class DiagnosticEngine* engine_;
    DiagnosticMessage diag_;
    bool emitted_ = false;

public:
    DiagnosticBuilder(DiagnosticEngine* engine, DiagnosticID id, 
                      DiagnosticSeverity severity, SourceLocation loc);
    
    // Move-only
    DiagnosticBuilder(DiagnosticBuilder&& other) noexcept;
    DiagnosticBuilder& operator=(DiagnosticBuilder&& other) noexcept;
    DiagnosticBuilder(const DiagnosticBuilder&) = delete;
    DiagnosticBuilder& operator=(const DiagnosticBuilder&) = delete;
    
    ~DiagnosticBuilder();

    /// Add string argument to format string
    DiagnosticBuilder& operator<<(std::string_view arg);
    
    /// Add integer argument
    DiagnosticBuilder& operator<<(int64_t arg);
    
    /// Add source range to highlight
    DiagnosticBuilder& operator<<(SourceRange range);

    /// Emit the diagnostic immediately
    void emit();
};

/// Central diagnostic reporting engine
class DiagnosticEngine {
public:
    using DiagnosticHandler = std::function<void(const DiagnosticMessage&)>;

private:
    const SourceManager* source_manager_;
    DiagnosticHandler handler_;
    
    uint32_t error_count_ = 0;
    uint32_t warning_count_ = 0;
    
    bool warnings_as_errors_ = false;
    bool suppress_warnings_ = false;
    uint32_t error_limit_ = 20;  // Stop after this many errors

public:
    explicit DiagnosticEngine(const SourceManager* sm);
    
    /// Set custom diagnostic handler (default prints to stderr)
    void set_handler(DiagnosticHandler handler);
    
    /// Report a diagnostic at a location
    DiagnosticBuilder report(DiagnosticID id, SourceLocation loc);
    
    /// Report a diagnostic without location
    DiagnosticBuilder report(DiagnosticID id);
    
    /// Emit a pre-built diagnostic
    void emit(const DiagnosticMessage& diag);
    
    // Configuration
    void set_warnings_as_errors(bool enable) { warnings_as_errors_ = enable; }
    void set_suppress_warnings(bool enable) { suppress_warnings_ = enable; }
    void set_error_limit(uint32_t limit) { error_limit_ = limit; }
    
    // Query state
    uint32_t error_count() const { return error_count_; }
    uint32_t warning_count() const { return warning_count_; }
    bool has_errors() const { return error_count_ > 0; }
    bool should_stop() const { return error_count_ >= error_limit_; }
    
    // Access source manager
    const SourceManager* source_manager() const { return source_manager_; }

private:
    void default_handler(const DiagnosticMessage& diag);
    std::string format_diagnostic(const DiagnosticMessage& diag) const;
};

} // namespace nova

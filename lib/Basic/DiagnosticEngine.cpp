// TODO: Implement diagnostic engine
#include "nova/Basic/DiagnosticEngine.hpp"

namespace nova {
    // Placeholder
    DiagnosticBuilder::DiagnosticBuilder(DiagnosticEngine* engine, DiagnosticID id,
                                        DiagnosticSeverity severity, SourceLocation loc)
                                        : engine_(engine),diag_({
                                            id,
                                            severity,
                                            loc,
                                            {},
                                            {}
                                        }){}
    DiagnosticBuilder::DiagnosticBuilder(DiagnosticBuilder&& other) noexcept
        : engine_(other.engine_), diag_(std::move(other.diag_)), emitted_(other.emitted_) {
            other.emitted_ = true;
            other.engine_ = nullptr;
        }

    DiagnosticBuilder& DiagnosticBuilder::operator=(DiagnosticBuilder&& other) noexcept {
        if (this != &other) {
            engine_ = other.engine_;
            diag_ = std::move(other.diag_);
            emitted_ = other.emitted_;
            other.emitted_ = true;
            other.engine_ = nullptr;
        }
        return * this;
    }

    DiagnosticBuilder::~DiagnosticBuilder() {
        if(!emitted_ && engine_) {
            emit();
        }
    }
    DiagnosticBuilder& DiagnosticBuilder::operator<<(std::string_view arg) {
        diag_.message += std::string(arg);
        return *this;
    }

    DiagnosticBuilder& DiagnosticBuilder::operator<<(int64_t arg) {
        diag_.message += std::to_string(arg);
        return *this;
    }

    DiagnosticBuilder& DiagnosticBuilder::operator<<(SourceRange range) {
        diag_.ranges.push_back(range);
        return *this;
    }

    void DiagnosticBuilder::emit() {
        if(engine_) {
            engine_->emit(diag_);
            emitted_ = true;
        }
    }

    DiagnosticEngine::DiagnosticEngine(const SourceManager* sm)
        : source_manager_(sm) {}

    void DiagnosticEngine::set_handler(DiagnosticHandler handler) {
        handler_ = std::move(handler);
    }

    DiagnosticBuilder DiagnosticEngine::report(DiagnosticID id, SourceLocation loc) {
        auto severity = get_default_severity(id);
        if (severity == DiagnosticSeverity::Warning && warnings_as_errors_) {
            severity = DiagnosticSeverity::Error;
        }
        return DiagnosticBuilder(this, id, severity, loc);
    }
    // Report a diagnostic without location
    // use invalid SourceLocation to indicate no location
    DiagnosticBuilder DiagnosticEngine::report(DiagnosticID id){
        return report(id, SourceLocation::invalid());
    }
    // emit a pre-built diagnostic to the handler
    void DiagnosticEngine::emit(const DiagnosticMessage& diag) {
        if(!handler_) {
            default_handler(diag);
        } else {
            handler_(diag);
        }
    }
    void DiagnosticEngine::default_handler(const DiagnosticMessage& diag) {
        std::string formatted = format_diagnostic(diag);
        switch (diag.severity) {
            case DiagnosticSeverity::Note:
                fprintf(stderr, "Note %s: %s\n", get_diagnostic_code(diag.id),formatted.c_str());
                break;
            case DiagnosticSeverity::Warning:
                if(!suppress_warnings_ && !warnings_as_errors_) {
                    fprintf(stderr, "Warning %s: %s\n", get_diagnostic_code(diag.id), formatted.c_str());
                    warning_count_++;
                }else if(warnings_as_errors_) {
                    fprintf(stderr, "Error (from warning) %s: %s\n", get_diagnostic_code(diag.id), formatted.c_str());
                    error_count_++;
                }
                break;
            case DiagnosticSeverity::Error:
                fprintf(stderr, "Error %s: %s\n", get_diagnostic_code(diag.id), formatted.c_str());
                error_count_++;
                break;
            case DiagnosticSeverity::Fatal:
                fprintf(stderr, "Fatal Error %s: %s\n", get_diagnostic_code(diag.id), formatted.c_str());
                //terminate the program
                std::terminate();
                break;
        }
    }
    // format: [file:] message at row:col
    std::string DiagnosticEngine::format_diagnostic(const DiagnosticMessage& diag) const {
        std::string result;
        uint32_t line, column;
        // Append location info 
        if(diag.location.is_valid()&& source_manager_) {
            source_manager_->get_line_column(diag.location, line, column);
            auto filename = source_manager_->get_filename(diag.location);
            result += std::string(filename);
            result += ":";
            result +=diag.message;
            result += " at ";
            result += std::to_string(line);
            result += ":";
            result += std::to_string(column);
            return result;
        }
        return diag.message;
    }
 
}


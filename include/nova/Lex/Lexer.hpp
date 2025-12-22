#pragma once
#include "Token.hpp"
#include "nova/Basic/SourceManager.hpp"
#include "nova/Basic/IdentifierTable.hpp"

namespace nova {
// 词法分析器
class Lexer {
private:
    const SourceManager& source_manager_;
    IdentifierTable& identifier_table_;

    uint16_t file_id_;
    const char* buffer_start_;
    const char* buffer_ptr_;
    const char* buffer_end_;

    bool at_start_of_line_;
    bool seen_space_;

public:
    Lexer(const SourceManager& sm, IdentifierTable& id_table, uint16_t file_id);

    void lex(Token& result);

private:
    void skip_whitespace_and_comments();
    void lex_identifier(Token& result, const char* start, SourceLocation loc);
    void lex_number(Token& result, const char* start, SourceLocation loc);
    void lex_string(Token& result, const char* start, SourceLocation loc);
    void lex_char(Token& result, const char* start, SourceLocation loc);

    void form_token(Token& result, TokenKind kind, const char* start, SourceLocation loc);

    char peek() const { return (buffer_ptr_ < buffer_end_) ? *buffer_ptr_ : '\0'; }

    static bool is_identifier_start(char c);
    static bool is_identifier_continue(char c);
    static bool is_digit(char c);
};

} // namespace nova

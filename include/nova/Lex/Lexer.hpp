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

    //at the start of a new line means previous char is '\n' 
    //use such flag to set Token::at_start_of_line flag
    bool at_start_of_line_;

    //whether we have seen whitespace before current token
    //use such flag to set Token::has_leading_space flag
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
    void lex_punctuation(Token& result, const char* start, SourceLocation loc);

    void form_token(Token& result, TokenKind kind, const char* start, SourceLocation loc);

    char peek() const { return (buffer_ptr_ < buffer_end_) ? *buffer_ptr_ : '\0'; }

    uint32_t get_current_offset() const {
        return static_cast<uint32_t>(buffer_ptr_ - buffer_start_);
    }

    static bool is_identifier_start(char c);
    static bool is_identifier_continue(char c);
    static bool is_digit(char c);
    static bool is_hex_digit(char c);
    static bool is_octal_digit(char c);
    static bool is_binary_digit(char c);
    static int hex_value(char c);
    static int octal_value(char c);
    static int binary_value(char c);
};

} // namespace nova

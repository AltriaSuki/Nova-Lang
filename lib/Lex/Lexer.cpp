#include "nova/Lex/Lexer.hpp"

#include <cstring>

namespace nova {
    Lexer::Lexer(const SourceManager& sm,IdentifierTable& id_table,uint16_t file_id)
        :source_manager_(sm),identifier_table_(id_table),file_id_(file_id),
            at_start_of_line_(true),seen_space_(false) {
                const FileEntry* file = source_manager_.get_file(file_id_);
                if(file){
                    buffer_start_ = file->content.data();
                    buffer_ptr_ = buffer_start_;
                    buffer_end_ = buffer_start_ + file->content.size();
                }else {
                    buffer_start_ = nullptr;
                    buffer_ptr_ = nullptr;
                    buffer_end_ = nullptr;
                }
            }
    // lex token and store result in 'result'
    // token include identifiers, keywords, literals, punctuations
    void Lexer::lex(Token& result){
        skip_whitespace_and_comments();
        if(buffer_ptr_ >= buffer_end_) {
            form_token(result,TokenKind::eof,buffer_ptr_,SourceLocation::create(file_id_,static_cast<uint32_t>(buffer_ptr_ - buffer_start_)));
            return;
        }
        const char current_char = peek();
        SourceLocation loc = SourceLocation::create(file_id_,get_current_offset());
        const char* start = buffer_ptr_;
        if(is_identifier_start(current_char)) {
            lex_identifier(result,start,loc);
            return;
        }else if(is_digit(current_char)) {
            lex_number(result,start,loc);
            return;
        }else if(current_char == '"'){
            lex_string(result,start,loc);
            return;
        }else if(current_char == '\''){
            lex_char(result,start,loc);
            return;
        }else {
            lex_punctuation(result,start,loc);
            return;
        }
    }

    void Lexer::skip_whitespace_and_comments(){
        seen_space_ = false;
        const char* cur=buffer_ptr_;
        const char* end=buffer_end_;
        while(cur < end) {
            if(is_whitespace(*cur)){
                seen_space_ = true;
                if(*cur == '\n')
                    at_start_of_line_ = true;
                ++cur;
            }else if(*cur == '/' && (cur + 1) < end){
                if(*(cur + 1) == '/'){
                    // single line comment
                    seen_space_ = true;
                    cur += 2;
                    const void* newline_pos =
                        std::memchr(cur, '\n', static_cast<size_t>(end - cur));
                    if(!newline_pos) {
                        cur = end;
                    } else {
                        cur = static_cast<const char*>(newline_pos) + 1;
                        at_start_of_line_ = true;
                    }
                }else if(*(cur + 1) == '*') {
                    //mutilple line comment
                    seen_space_ = true;
                    cur += 2;
                    while(cur < end) {
                        const void* star_pos =
                            std::memchr(cur, '*', static_cast<size_t>(end - cur));
                        if(!star_pos) {
                            if(std::memchr(cur, '\n', static_cast<size_t>(end - cur))) {
                                at_start_of_line_ = true;
                            }
                            cur = end; // unterminated comment: consume to EOF
                            break;
                        }
                        const char* star = static_cast<const char*>(star_pos);
                        const void* newline_in_chunk =
                            std::memchr(cur, '\n', static_cast<size_t>(star - cur));
                        if(newline_in_chunk) {
                            at_start_of_line_ = true;
                        }
                        if((star + 1) < end && *(star + 1) == '/') {
                            cur = star + 2;
                            break;
                        }
                        cur = star + 1;
                    }

                }else {
                    break;//invalid comment start
                }
            } else {
                break;
            }
        }
        buffer_ptr_ = cur;
    }

    int Lexer::hex_value(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        } else if (c >= 'a' && c <= 'f') {
            return 10 + (c - 'a');
        } else if (c >= 'A' && c <= 'F') {
            return 10 + (c - 'A');
        }
        return 0; // should not reach here
    }
    
    int Lexer::octal_value(char c) {
        return c - '0';
    }

    int Lexer::binary_value(char c) {
        return c - '0';
    }
    
    void Lexer::form_token(Token& result,TokenKind kind,const char* start,SourceLocation loc) {
        result.set_kind(kind);
        result.set_location(loc);
        result.set_length(static_cast<uint32_t>(buffer_ptr_ - start));
        if(at_start_of_line_){
            result.set_flag_at_start_of_line();
            at_start_of_line_ = false;
        }else if(seen_space_){
            result.set_flag_has_leading_space();
            seen_space_ = false;
        }
    }

    void Lexer::lex_identifier(Token& result,const char* start,SourceLocation loc) {
        const char* cur = buffer_ptr_;
        const char* end = buffer_end_;
        while(cur < end && is_identifier_continue(*cur)) 
            ++cur;
        std::string_view ident_text(start,static_cast<size_t>(cur - start));
        //check if it's a keyword
        IdentifierInfo* info = identifier_table_.get(ident_text);
        buffer_ptr_ = cur;
        if(info && info->is_keyword){
            form_token(result,info->token_kind,start,loc);
            result.set_identifier_info(info);
        }else if(!info){
            identifier_table_.add_identifier(ident_text);
            info = identifier_table_.get(ident_text);
            form_token(result,TokenKind::identifier,start,loc);
            result.set_identifier_info(info);
        }else {
            //this identifier already exists but not a keyword
            form_token(result,TokenKind::identifier,start,loc);
            result.set_identifier_info(info);
        }
    }

    void Lexer::lex_number(Token& result,const char* start,SourceLocation loc){
        //decimal,hexadecimal,octal,binary literals
        //and floating point literals including scientific notation
        bool is_float = false;
        const char* cur = buffer_ptr_;
        const char* end = buffer_end_;
        if(*cur == '0' && (cur + 1) < end) {
            char next_char = *(cur + 1);
            if(next_char == 'x' || next_char == 'X') {
                cur += 2;
                while(cur < end && is_hex_digit(*cur)) 
                    ++cur;   
            }else if(next_char == 'b' || next_char == 'B') {
                cur += 2;
                while(cur < end && is_binary_digit(*cur)) 
                    ++cur;
            }else if(next_char == 'o' || next_char == 'O') {
                cur += 2;
                while(cur < end && is_octal_digit(*cur)) 
                    ++cur;
            }
        }else {
            while(cur < end && is_digit(*cur))
                ++cur;
            //check for floating point
            if(cur < end && *cur == '.') {
                is_float = true;
                ++cur;
                while(cur < end && is_digit(*cur)) 
                    ++cur;
            }
            //scientific notation
            if(cur < end && (*cur == 'e' || *cur == 'E')) {
                is_float = true;
                ++cur;
                if(cur < end && (*cur == '+' || *cur == '-'))
                    ++cur;
                while(cur < end && is_digit(*cur))
                    ++cur;
            }
        }
        buffer_ptr_ = cur;
        if(is_float)
            form_token(result,TokenKind::floating_constant,start,loc);
        else
            form_token(result,TokenKind::numeric_constant,start,loc);
    }

    void Lexer::lex_string(Token& result,const char* start,SourceLocation loc) {
        const char* cur = buffer_ptr_;
        const char* end = buffer_end_;
        cur++; //skip opening "
        while(cur < end) {
            if(*cur == '"') {
                cur++;
                break;
            }else if(*cur == '\\') {
                ++cur; //skip '\'
                if(cur < buffer_end_) 
                    ++cur; //skip escaped char
            } else {
                ++cur;
            }
        }
        buffer_ptr_ = cur;
        form_token(result,TokenKind::string_literal,start,loc);
    }
    //char is unicode code point enclosed in single quotes
    void Lexer::lex_char(Token& result,const char* start,SourceLocation loc) {
        
        const char* cur = buffer_ptr_;
        const char* end = buffer_end_;
        cur++;
        while(cur < end) {
            if(*cur == '\'') {
                cur++;
                //current char is closing '
                break;
            }else if(*cur == '\\') {
                ++cur;
                if(cur < buffer_end_)
                    ++cur;
            }else {
                ++cur;
            }
        }
        //token example: 'a', '\n', '\u1234'
        buffer_ptr_ = cur;
        form_token(result,TokenKind::char_constant,start,loc);

    }

    void Lexer::lex_punctuation(Token& result,const char* start,SourceLocation loc) {
        TokenKind kind = TokenKind::unknown;
        const char* cur=buffer_ptr_;
        const char* end = buffer_end_;
        if(cur >= end) {
            form_token(result,kind,start,loc);
            return;
        }

        const char c = *cur++;
#include "nova/Lex/Punctuation.inc"

        if(kind == TokenKind::unknown) {
            //unknown punctuation, consume char until whitespace
            while(cur < buffer_end_ && !is_whitespace(*cur))
                ++cur;
        }
        buffer_ptr_ = cur;
        form_token(result,kind,start,loc);
        
    }

    //hitherto all functions are implemented
}

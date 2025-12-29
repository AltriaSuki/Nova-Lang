#include"nova/Lex/Lexer.hpp"

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
    namespace{
        bool is_whitespace(char c){
            return c==' ' || c=='\t' || c=='\n' || c=='\r';
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
        while(buffer_ptr_ < buffer_end_) {
            if(is_whitespace(*buffer_ptr_)){
                seen_space_ = true;
                if(*buffer_ptr_ == '\n')
                    at_start_of_line_ = true;
                ++buffer_ptr_;
            }else if(*buffer_ptr_ == '/' && (buffer_ptr_ + 1) < buffer_end_){
                if(*(buffer_ptr_ + 1) == '/'){
                    // single line comment
                    buffer_ptr_ += 2;
                    while(buffer_ptr_ < buffer_end_ && *buffer_ptr_ !='\n')
                        ++buffer_ptr_;
                    //current char is '\n' or end of buffer
                    ++buffer_ptr_;
                    at_start_of_line_ = true;
                }else if(*(buffer_ptr_ + 1) == '*') {
                    //mutilple line comment
                    buffer_ptr_ += 2;
                    while(buffer_ptr_ + 1 < buffer_end_){
                        if(*buffer_ptr_ == '*' && *(buffer_ptr_ + 1) == '/'){
                            buffer_ptr_ += 2;
                            break;
                        }
                        ++buffer_ptr_;
                    }

                }else {
                    break;//invalid comment start
                }
            }
        }
    }

    bool Lexer::is_identifier_start(char c){
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    bool Lexer::is_identifier_continue(char c){
        return is_identifier_start(c) || is_digit(c);
    }

    bool Lexer::is_digit(char c){
        return c >= '0' && c <= '9';
    }
    bool Lexer::is_hex_digit(char c) {
        return (c >= '0' && c <= '9') ||
                (c >= 'a' && c <= 'f') ||
                (c >= 'A' && c <= 'F');
    }
    bool Lexer::is_octal_digit(char c) {
        return c >= '0' && c <= '7';
    }
    bool Lexer::is_binary_digit(char c) {
        return c == '0' || c == '1';
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
        while(buffer_ptr_ < buffer_end_ && is_identifier_continue(*buffer_ptr_)) 
            ++buffer_ptr_;
        std::string_view ident_text(start,static_cast<size_t>(buffer_ptr_ - start));
        //check if it's a keyword
        IdentifierInfo* info = identifier_table_.get(ident_text);
        if(info && info->is_keyword){
            form_token(result,info->token_kind,start,loc);
            result.set_identifier_info(info);
        }else if(!info){
            identifier_table_.add_identifier(ident_text.data());
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
        if(*buffer_ptr_ == '0' && (buffer_ptr_ + 1) < buffer_end_) {
            char next_char = *(buffer_ptr_ + 1);
            if(next_char == 'x' || next_char == 'X') {
                buffer_ptr_ += 2;
                while(buffer_ptr_ < buffer_end_ && is_hex_digit(*buffer_ptr_)) 
                    ++buffer_ptr_;   
            }else if(next_char == 'b' || next_char == 'B') {
                buffer_ptr_ += 2;
                while(buffer_ptr_ < buffer_end_ && is_binary_digit(*buffer_ptr_)) 
                    ++buffer_ptr_;
            }else if(next_char == 'o' || next_char == 'O') {
                buffer_ptr_ += 2;
                while(buffer_ptr_ < buffer_end_ && is_octal_digit(*buffer_ptr_)) 
                    ++buffer_ptr_;
            }
        }else {
            while(buffer_ptr_ < buffer_end_ && is_digit(*buffer_ptr_))
                ++buffer_ptr_;
            //check for floating point
            if(buffer_ptr_ < buffer_end_ && *buffer_ptr_ == '.') {
                is_float = true;
                ++buffer_ptr_;
                while(buffer_ptr_ < buffer_end_ && isdigit(*buffer_ptr_)) 
                    ++buffer_ptr_;
            }
            //scientific notation
            if(buffer_ptr_ < buffer_end_ && (*buffer_ptr_ == 'e' || *buffer_ptr_ == 'E')) {
                is_float = true;
                ++buffer_ptr_;
                if(buffer_ptr_ < buffer_end_ && (*buffer_ptr_ == '+') || *buffer_ptr_ == '-') 
                    ++buffer_ptr_;
                while(buffer_ptr_ < buffer_end_ && isdigit(*buffer_ptr_))
                    ++buffer_ptr_;
            }
        }
        if(is_float)
            form_token(result,TokenKind::floating_constant,start,loc);
        else
            form_token(result,TokenKind::numeric_constant,start,loc);
    }

    void Lexer::lex_string(Token& result,const char* start,SourceLocation loc) {
        buffer_ptr_++; //skip opening "
        while(buffer_ptr_ < buffer_end_) {
            if(*buffer_ptr_ == '"') {
                buffer_ptr_++;
                break;
            }else if(*buffer_ptr_ == '\\') {
                ++buffer_ptr_; //skip '\'
                if(buffer_ptr_ < buffer_end_) 
                    ++buffer_ptr_; //skip escaped char
            } else {
                ++buffer_ptr_;
            }
        }
        form_token(result,TokenKind::string_literal,start,loc);
    }
    //char is unicode code point enclosed in single quotes
    void Lexer::lex_char(Token& result,const char* start,SourceLocation loc) {
        buffer_ptr_++;
        while(buffer_ptr_ < buffer_end_) {
            if(*buffer_ptr_ == '\'') {
                buffer_ptr_++;
                //current char is closing '
                break;
            }else if(*buffer_ptr_ == '\\') {
                ++buffer_ptr_;
                if(buffer_ptr_ < buffer_end_)
                    ++buffer_ptr_;
            }else {
                ++buffer_ptr_;
            }
        }
        //token example: 'a', '\n', '\u1234'
        form_token(result,TokenKind::char_constant,start,loc);
    }

    void Lexer::lex_punctuation(Token& result,const char* start,SourceLocation loc) {
        //current maximum punctuation length is 2
        uint8_t max_punct_length = 2;
        uint8_t length = 0;
        TokenKind kind = TokenKind::unknown;
        for(uint8_t len = max_punct_length; len >= 1; --len) {
            std::string_view punct_text(start,len);
            bool found = false;
            #define NOVA_PUNCT(name, spelling) \
                if(punct_text == spelling) {\
                    kind = TokenKind::name;\
                    length = len;\
                    found = true;\
                }
            #include "nova/Lex/TokenKinds.def"
            #undef NOVA_PUNCT
            if(found) {
                buffer_ptr_ += length;
                form_token(result,kind,start,loc);
                return;
            }
        }
        //unknown punctuation, consume char until whitespace
        while(buffer_ptr_ < buffer_end_ && !is_whitespace(*buffer_ptr_)) 
            ++buffer_ptr_;
        form_token(result,TokenKind::unknown,start,loc);
    }

    //hitherto all functions are implemented
}

#pragma once
#include "TokenKinds.hpp"
#include "nova/Basic/SourceLocation.hpp"

namespace nova {

struct IdentifierInfo;

class Token {
private:
    SourceLocation loc_;
    uint32_t length_;
    TokenKind kind_;

    struct Flags {
        // whether the token is at the start of a new line, useful for formatting
        bool at_start_of_line : 1;
        // whether the token has leading whitespace, useful for formatting
        bool has_leading_space : 1;
        uint8_t reserved : 6;
    } flags_;

    IdentifierInfo* identifier_info_;

public:
    Token() : loc_(SourceLocation::invalid()), length_(0),
              kind_(TokenKind::unknown), flags_{false, false, 0},
              identifier_info_(nullptr) {}

    TokenKind get_kind() const { return kind_; }
    SourceLocation get_location() const { return loc_; }
    uint32_t get_length() const { return length_; }
    // plus length to get the end location
    SourceRange get_source_range() const {
        return SourceRange(loc_, loc_.get_offset_location(length_));
    }

    IdentifierInfo* get_identifier_info() const { return identifier_info_; }

    bool at_start_of_line() const { return flags_.at_start_of_line; }
    bool has_leading_space() const { return flags_.has_leading_space; }

    void set_kind(TokenKind kind) { kind_ = kind; }
    void set_location(SourceLocation loc) { loc_ = loc; }
    void set_length(uint32_t len) { length_ = len; }
    void set_identifier_info(IdentifierInfo* info) { identifier_info_ = info; }

    void set_flag_at_start_of_line() { flags_.at_start_of_line = true; }
    void clear_flag_at_start_of_line() { flags_.at_start_of_line = false; }
    void set_flag_has_leading_space() { flags_.has_leading_space = true; }
    void clear_flag_has_leading_space() { flags_.has_leading_space = false; }

    bool is(TokenKind k) const { return kind_ == k; }
    bool is_not(TokenKind k) const { return kind_ != k; }

    bool is_one_of(TokenKind k1, TokenKind k2) const {
        return is(k1) || is(k2);
    }
    //recursive variadic template
    template<typename... Ts>
    bool is_one_of(TokenKind k1, TokenKind k2, Ts... ks) const {
        return is(k1) || is_one_of(k2, ks...);
    }

    bool is_literal() const {
        return kind_ == TokenKind::numeric_constant ||
               kind_ == TokenKind::string_literal ||
               kind_ == TokenKind::char_constant ||
               kind_ == TokenKind::floating_constant;
    }
};

} // namespace nova

#pragma once

namespace nova {

enum class TokenKind {
    // Keywords
    kw_func, kw_let, kw_mut, kw_class, kw_trait, kw_impl,
    kw_if, kw_else, kw_match, kw_while, kw_for, kw_return,
    kw_pub, kw_priv, kw_mod, kw_use, kw_unsafe,
    kw_true, kw_false,

    // Type keywords
    kw_i8, kw_i16, kw_i32, kw_i64,
    kw_u8, kw_u16, kw_u32, kw_u64,
    kw_f32, kw_f64,
    kw_bool, kw_str, kw_char,

    // Punctuation
    plus, minus, star, slash, percent,
    equal, equalequal, exclaimequal,
    less, lessequal, greater, greaterequal,
    amp, ampamp,
    pipe, pipepipe,
    arrow, fatarrow,
    period, coloncolon,

    comma, semi, colon,
    l_paren, r_paren,
    l_brace, r_brace,
    l_square, r_square,

    // Literals
    numeric_constant,
    string_literal,
    char_constant,

    // Identifiers
    identifier,

    // Special
    eof,
    unknown
};

const char* get_token_name(TokenKind kind);
const char* get_punctuation_spelling(TokenKind kind);

} // namespace nova

#include "nova/Lex/TokenKinds.hpp"
namespace nova {
    const char* get_token_name(TokenKind kind) {
        switch(kind) {
            case TokenKind::kw_func: return "func";
            case TokenKind::kw_let: return "let";
            case TokenKind::kw_mut: return "mut";
            case TokenKind::kw_class: return "class";
            case TokenKind::kw_trait: return "trait";
            case TokenKind::kw_impl: return "impl";
            case TokenKind::kw_if: return "if";
            case TokenKind::kw_else: return "else";
            case TokenKind::kw_match: return "match";
            case TokenKind::kw_while: return "while";
            case TokenKind::kw_for: return "for";
            case TokenKind::kw_return: return "return";
            case TokenKind::kw_pub: return "pub";
            case TokenKind::kw_priv: return "priv";
            case TokenKind::kw_mod: return "mod";
            case TokenKind::kw_use: return "use";
            case TokenKind::kw_unsafe: return "unsafe";
            case TokenKind::kw_true: return "true";
            case TokenKind::kw_false: return "false";
            case TokenKind::kw_i8: return "i8";
            case TokenKind::kw_i16: return "i16";
            case TokenKind::kw_i32: return "i32";
            case TokenKind::kw_i64: return "i64";
            case TokenKind::kw_u8: return "u8";
            case TokenKind::kw_u16: return "u16";
            case TokenKind::kw_u32: return "u32";
            case TokenKind::kw_u64: return "u64";
            case TokenKind::kw_f32: return "f32";
            case TokenKind::kw_f64: return "f64";
            case TokenKind::kw_bool: return "bool";
            case TokenKind::kw_str: return "str";
            case TokenKind::kw_char: return "char";
            case TokenKind::plus: return "plus";
            case TokenKind::minus: return "minus";
            case TokenKind::star: return "star";
            case TokenKind::slash: return "slash";
            case TokenKind::percent: return "percent";
            case TokenKind::equal: return "equal";
            case TokenKind::equalequal: return "equalequal";
            case TokenKind::exclaimequal: return "exclaimequal";
            case TokenKind::less: return "less";
            case TokenKind::lessequal: return "lessequal";
            case TokenKind::greater: return "greater";
            case TokenKind::greaterequal: return "greaterequal";
            case TokenKind::amp: return "amp";
            case TokenKind::ampamp: return "ampamp";
            case TokenKind::pipe: return "pipe";
            case TokenKind::pipepipe: return "pipepipe";
            case TokenKind::arrow: return "arrow";
            case TokenKind::fatarrow: return "fatarrow";
            case TokenKind::period: return "period";
            case TokenKind::coloncolon: return "coloncolon";
            case TokenKind::comma: return "comma";
            case TokenKind::semi: return "semi";
            case TokenKind::colon: return "colon";
            case TokenKind::l_paren: return "l_paren";
            case TokenKind::r_paren: return "r_paren";
            case TokenKind::l_brace: return "l_brace";
            case TokenKind::r_brace: return "r_brace";
            case TokenKind::l_square: return "l_square";
            case TokenKind::r_square: return "r_square";
            case TokenKind::numeric_constant: return "numeric constant";
            case TokenKind::string_literal: return "string literal";
            case TokenKind::char_constant: return "char constant";
            case TokenKind::identifier: return "identifier";
            case TokenKind::eof: return "end of file";
            case TokenKind::unknown: return "unknown";
            case TokenKind::count: return nullptr;
        }
    }

    const char* get_punctuation_spelling(TokenKind kind) {
        switch(kind) {
            case TokenKind::plus: return "+";
            case TokenKind::minus: return "-";
            case TokenKind::star: return "*";
            case TokenKind::slash: return "/";
            case TokenKind::percent: return "%";
            case TokenKind::equal: return "=";
            case TokenKind::equalequal: return "==";
            case TokenKind::exclaimequal: return "!=";
            case TokenKind::less: return "<";
            case TokenKind::lessequal: return "<=";
            case TokenKind::greater: return ">";
            case TokenKind::greaterequal: return ">=";
            case TokenKind::amp: return "&";
            case TokenKind::ampamp: return "&&";
            case TokenKind::pipe: return "|";
            case TokenKind::pipepipe: return "||";
            case TokenKind::arrow: return "->";
            case TokenKind::fatarrow: return "=>";
            case TokenKind::period: return ".";
            case TokenKind::coloncolon: return "::";
            case TokenKind::comma: return ",";
            case TokenKind::semi: return ";";
            case TokenKind::colon: return ":";
            case TokenKind::l_paren: return "(";
            case TokenKind::r_paren: return ")";
            case TokenKind::l_brace: return "{";
            case TokenKind::r_brace: return "}";
            case TokenKind::l_square: return "[";
            case TokenKind::r_square: return "]";
            default: return nullptr;
        }
    }


}
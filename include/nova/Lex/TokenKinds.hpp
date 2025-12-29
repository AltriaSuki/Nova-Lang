#pragma once

namespace nova {

enum class TokenKind {
#define NOVA_KEYWORD(name, spelling) name,
#define NOVA_TYPE_KEYWORD(name, spelling) name,
#define NOVA_PUNCT(name, spelling) name,
#define NOVA_LITERAL(name, token_name) name,
#define NOVA_TOKEN(name, token_name) name,
#include "nova/Lex/TokenKinds.def"
#undef NOVA_TOKEN
#undef NOVA_LITERAL
#undef NOVA_PUNCT
#undef NOVA_TYPE_KEYWORD
#undef NOVA_KEYWORD
    count,
};
//this is english word of the token
const char* get_token_name(TokenKind kind);
//this is punctuation of the token
const char* get_punctuation_spelling(TokenKind kind);

} // namespace nova

#include "nova/Lex/TokenKinds.hpp"

namespace nova {
    //helper arrays only visible in this translation unit
namespace {

static constexpr const char* kTokenNames[] = {
#define NOVA_KEYWORD(name, spelling) spelling,
#define NOVA_TYPE_KEYWORD(name, spelling) spelling,
#define NOVA_PUNCT(name, spelling) #name,
#define NOVA_LITERAL(name, token_name) token_name,
#define NOVA_TOKEN(name, token_name) token_name, 
#include "nova/Lex/TokenKinds.def"
#undef NOVA_TOKEN
#undef NOVA_LITERAL
#undef NOVA_PUNCT
#undef NOVA_TYPE_KEYWORD
#undef NOVA_KEYWORD
};

static constexpr const char* kPunctuationSpellings[] = {
#define NOVA_KEYWORD(name, spelling) nullptr,
#define NOVA_TYPE_KEYWORD(name, spelling) nullptr,
#define NOVA_PUNCT(name, spelling) spelling,
#define NOVA_LITERAL(name, token_name) nullptr,
#define NOVA_TOKEN(name, token_name) nullptr,
#include "nova/Lex/TokenKinds.def"
#undef NOVA_TOKEN
#undef NOVA_LITERAL
#undef NOVA_PUNCT
#undef NOVA_TYPE_KEYWORD
#undef NOVA_KEYWORD
};

static constexpr unsigned kNumTokenKinds =
    static_cast<unsigned>(sizeof(kTokenNames) / sizeof(kTokenNames[0]));
// TokenKind starts from 0 to count-1 and is continuous
static_assert(kNumTokenKinds == static_cast<unsigned>(TokenKind::count),
              "TokenKind table size must match TokenKind::count");
static_assert(sizeof(kTokenNames) / sizeof(kTokenNames[0]) ==
                  sizeof(kPunctuationSpellings) / sizeof(kPunctuationSpellings[0]),
              "TokenKind tables must stay in sync");

} // namespace

const char* get_token_name(TokenKind kind) {
    auto index = static_cast<unsigned>(kind);
    if (index >= kNumTokenKinds) {
        return nullptr;
    }
    return kTokenNames[index];
}

const char* get_punctuation_spelling(TokenKind kind) {
    auto index = static_cast<unsigned>(kind);
    if (index >= kNumTokenKinds) {
        return nullptr;
    }
    return kPunctuationSpellings[index];
}

} // namespace nova

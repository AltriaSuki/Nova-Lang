#include "nova/Basic/IdentifierTable.hpp"
#include "nova/Lex/TokenKinds.hpp"
#include <cstddef>

namespace nova {
    void IdentifierTable::add_keyword(const char* name, TokenKind kind) {
        auto info = std::make_unique<IdentifierInfo>(name, kind, true);
        table_.emplace(info->name, info.get());  // Use string as key
        storage_.push_back(std::move(info));
    }

    IdentifierTable::IdentifierTable() {
        // Seed the table with reserved keywords/type-keywords.
#define NOVA_KEYWORD(name, spelling) add_keyword(spelling, TokenKind::name);
#define NOVA_TYPE_KEYWORD(name, spelling) add_keyword(spelling, TokenKind::name);
#define NOVA_PUNCT(name, spelling)
#define NOVA_LITERAL(name, token_name)
#define NOVA_TOKEN(name, token_name)
#include "nova/Lex/TokenKinds.def"
#undef NOVA_TOKEN
#undef NOVA_LITERAL
#undef NOVA_PUNCT
#undef NOVA_TYPE_KEYWORD
#undef NOVA_KEYWORD
	    }

    void IdentifierTable::add_identifier(std::string_view name) {
        auto info = std::make_unique<IdentifierInfo>(std::string(name), TokenKind::identifier, false);
        table_.emplace(info->name, info.get());
        storage_.push_back(std::move(info));
    }
    //
    IdentifierInfo* IdentifierTable::get(std::string_view name) {
        // C++20 transparent lookup - can search with string_view in string map
        auto it = table_.find(name);
        if (it != table_.end()) {
            return it->second;
        }
        return nullptr;
    }
}

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
        for (std::size_t i = 0; i < static_cast<std::size_t>(TokenKind::count); ++i) {
            TokenKind kind = static_cast<TokenKind>(i);
            const char* name = get_token_name(kind);
            if (name) {
                add_keyword(name, kind);
            }
        }
    }

    void IdentifierTable::add_identifier(const char* name) {
        add_keyword(name, TokenKind::identifier);
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
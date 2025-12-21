#pragma once
#include "nova/Lex/TokenKinds.hpp"
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <memory>

namespace nova {

struct IdentifierInfo {
    std::string name;
    TokenKind token_kind;
    bool is_keyword;

    IdentifierInfo(std::string n, TokenKind kind, bool keyword)
        : name(std::move(n)), token_kind(kind), is_keyword(keyword) {}
};

class IdentifierTable {
private:
    std::unordered_map<std::string_view, IdentifierInfo*> table_;
    std::vector<std::unique_ptr<IdentifierInfo>> storage_;

public:
    IdentifierTable();

    IdentifierInfo* get(std::string_view name);

private:
    void add_keyword(const char* name, TokenKind kind);
};

} // namespace nova

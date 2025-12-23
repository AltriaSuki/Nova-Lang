#pragma once
#include "nova/Lex/TokenKinds.hpp"
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <memory>

namespace nova {

// C++20 transparent hash for heterogeneous lookup
struct StringHash {
    using is_transparent = void;
    
    [[nodiscard]] std::size_t operator()(std::string_view sv) const noexcept {
        return std::hash<std::string_view>{}(sv);
    }
};

struct StringEqual {
    using is_transparent = void;
    
    [[nodiscard]] bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
        return lhs == rhs;
    }
};

struct IdentifierInfo {
    std::string name;
    TokenKind token_kind;
    bool is_keyword;

    IdentifierInfo(std::string n, TokenKind kind, bool keyword)
        : name(std::move(n)), token_kind(kind), is_keyword(keyword) {}
};

class IdentifierTable {
private:
    // Use std::string as key (owns the data), with transparent lookup
    std::unordered_map<std::string, IdentifierInfo*, StringHash, StringEqual> table_;
    std::vector<std::unique_ptr<IdentifierInfo>> storage_;

public:
    IdentifierTable();

    [[nodiscard]] IdentifierInfo* get(std::string_view name);
    void add_identifier(const char* name);

private:
    void add_keyword(const char* name, TokenKind kind);
};

} // namespace nova

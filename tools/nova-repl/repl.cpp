#include "nova/Basic/SourceManager.hpp"
#include "nova/Basic/IdentifierTable.hpp"
#include "nova/Lex/Lexer.hpp"
#include <iostream>
#include <string>

int main() {
    std::cout << "Nova REPL v0.1\n";
    std::cout << "Type 'exit' to quit\n\n";

    nova::SourceManager source_mgr;
    nova::IdentifierTable id_table;

    std::string line;
    while (true) {
        std::cout << ">>> ";
        if (!std::getline(std::cin, line)) break;

        if (line == "exit" || line == "quit") break;
        if (line.empty()) continue;

        uint16_t file_id = source_mgr.add_file("<repl>", line);
        nova::Lexer lexer(source_mgr, id_table, file_id);

        while (true) {
            nova::Token token;
            lexer.lex(token);

            if (token.get_kind() == nova::TokenKind::eof) break;

            std::string_view text = source_mgr.get_text(token.get_source_range());
            std::cout << nova::get_token_name(token.get_kind()) << "(" << text << ") ";
        }
        std::cout << "\n";
    }

    return 0;
}

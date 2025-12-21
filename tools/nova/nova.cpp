#include "nova/Basic/SourceManager.hpp"
#include "nova/Basic/IdentifierTable.hpp"
#include "nova/Lex/Lexer.hpp"
#include "nova/Lex/Token.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: nova <file.nova>\n";
        return 1;
    }

    // Read file
    std::string filename = argv[1];
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Cannot open file: " << filename << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Setup compiler components
    nova::SourceManager source_mgr;
    nova::IdentifierTable id_table;

    uint16_t file_id = source_mgr.add_file(filename, source);

    // Lex
    nova::Lexer lexer(source_mgr, id_table, file_id);

    std::cout << "Tokens:\n";
    std::cout << "-------\n";

    while (true) {
        nova::Token token;
        lexer.lex(token);

        if (token.get_kind() == nova::TokenKind::eof) {
            break;
        }

        std::cout << nova::get_token_name(token.get_kind());

        std::string_view text = source_mgr.get_text(token.get_source_range());
        std::cout << "('" << text << "')";

        std::cout << " at " << source_mgr.format_location(token.get_location());
        std::cout << "\n";
    }

    return 0;
}

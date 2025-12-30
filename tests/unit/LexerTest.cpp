#include <gtest/gtest.h>
#include "nova/Lex/Lexer.hpp"
#include "nova/Basic/SourceManager.hpp"
#include "nova/Basic/IdentifierTable.hpp"
#define NOVA_SNIPPET R"(
        // This is a comment
        func main() {
            let x = 42;
            let y = 0x2A;
            let z = "Hello, World!";
            let c = 'a';

        }
        )"

namespace nova {
    TEST(LexerTest, BasicLexing) {
        SourceManager sm;

        //rust code snippet
        const char *code = R"(
        // This is a comment
        func main() {
            let x = 42;
            let y = 0x2A;
            let z = "Hello, World!";
            let c = 'a';

        }
        )";
        uint16_t file_id = sm.add_file("test.nova", code);
        IdentifierTable id_table;
        Lexer lexer(sm, id_table, file_id);
        Token token;
        std::vector<TokenKind> expected_kinds = {
            TokenKind::kw_func,
            TokenKind::identifier,
            TokenKind::l_paren,
            TokenKind::r_paren,
            TokenKind::l_brace,
            TokenKind::kw_let,
            TokenKind::identifier,
            TokenKind::equal,
            TokenKind::numeric_constant,
            TokenKind::semi,
            TokenKind::kw_let,
            TokenKind::identifier,
            TokenKind::equal,
            TokenKind::numeric_constant,
            TokenKind::semi,
            TokenKind::kw_let,
            TokenKind::identifier,
            TokenKind::equal,
            TokenKind::string_literal,
            TokenKind::semi,
            TokenKind::kw_let,
            TokenKind::identifier,
            TokenKind::equal,
            TokenKind::char_constant,
            TokenKind::semi,
            TokenKind::r_brace,
            TokenKind::eof
        };
        for (TokenKind expected_kind : expected_kinds) {
            lexer.lex(token);
            EXPECT_EQ(token.get_kind(), expected_kind);
        }
    }

    TEST(UNICODETEST,UnicodeCharLiterals) {
        SourceManager sm;
        const char* code = R"(
        func main() {
            let c1 = 'a';
            let c2 = '\n';
            let c3 = '\u03A9'; // Greek capital letter Omega
            let c4 = '\U0001F600'; // Grinning face emoji
        }
        )";
        uint16_t file_id = sm.add_file("unicode_test.nova", code);
        IdentifierTable id_table;
        Lexer lexer(sm, id_table, file_id);
        Token token;
        std::vector<TokenKind> expected_kinds = {
            TokenKind::kw_func,
            TokenKind::identifier,
            TokenKind::l_paren,
            TokenKind::r_paren,
            TokenKind::l_brace,
            TokenKind::kw_let,
            TokenKind::identifier,
            TokenKind::equal,
            TokenKind::char_constant,
            TokenKind::semi,
            TokenKind::kw_let,
            TokenKind::identifier,
            TokenKind::equal,
            TokenKind::char_constant,
            TokenKind::semi,
            TokenKind::kw_let,
            TokenKind::identifier,
            TokenKind::equal,
            TokenKind::char_constant,
            TokenKind::semi,
            TokenKind::kw_let,
            TokenKind::identifier,
            TokenKind::equal,
            TokenKind::char_constant,
            TokenKind::semi,
            TokenKind::r_brace,
            TokenKind::eof
        };
        for (TokenKind expected_kind : expected_kinds) {
            lexer.lex(token);
            EXPECT_EQ(token.get_kind(), expected_kind);
        }
    }
}

#include <gtest/gtest.h>
#include "nova/Lex/Lexer.hpp"
#include "nova/Basic/SourceManager.hpp"
#include "nova/Basic/IdentifierTable.hpp"

TEST(LexerTest, BasicTokenization) {
    nova::SourceManager sm;
    nova::IdentifierTable id_table;

    std::string source = "func main() { let x = 42; }";
    uint16_t file_id = sm.add_file("test.nova", source);

    nova::Lexer lexer(sm, id_table, file_id);

    nova::Token token;
    lexer.lex(token);
    EXPECT_EQ(token.get_kind(), nova::TokenKind::kw_func);

    lexer.lex(token);
    EXPECT_EQ(token.get_kind(), nova::TokenKind::identifier);
}

TEST(LexerTest, NumberLiteral) {
    nova::SourceManager sm;
    nova::IdentifierTable id_table;

    std::string source = "42 3.14";
    uint16_t file_id = sm.add_file("test.nova", source);

    nova::Lexer lexer(sm, id_table, file_id);

    nova::Token token;
    lexer.lex(token);
    EXPECT_EQ(token.get_kind(), nova::TokenKind::numeric_constant);
    EXPECT_EQ(sm.get_text(token.get_source_range()), "42");
}

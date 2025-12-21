add_library(novaLex
    TokenKinds.cpp
    Token.cpp
    Lexer.cpp
)

target_link_libraries(novaLex PUBLIC
    novaBasic
)

target_include_directories(novaLex PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

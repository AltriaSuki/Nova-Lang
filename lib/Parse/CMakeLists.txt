add_library(novaParse
    Parser.cpp
)

target_link_libraries(novaParse PUBLIC
    novaBasic
    novaLex
    novaAST
)

target_include_directories(novaParse PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

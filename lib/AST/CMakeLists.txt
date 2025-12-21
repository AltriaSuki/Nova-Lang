add_library(novaAST
    ASTContext.cpp
)

target_link_libraries(novaAST PUBLIC
    novaBasic
)

target_include_directories(novaAST PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

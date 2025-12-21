add_library(novaSema
    Sema.cpp
)

target_link_libraries(novaSema PUBLIC
    novaBasic
    novaAST
)

target_include_directories(novaSema PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

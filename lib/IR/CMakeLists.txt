add_library(novaIR
    IR.cpp
    IRBuilder.cpp
    Module.cpp
)
target_link_libraries(novaIR PUBLIC novaBasic novaAST)
target_include_directories(novaIR PUBLIC ${PROJECT_SOURCE_DIR}/include)

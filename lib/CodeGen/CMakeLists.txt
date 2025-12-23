add_library(novaCodeGen
    CodeGenModule.cpp
)

target_link_libraries(novaCodeGen PUBLIC
    novaBasic
    novaAST
)

target_include_directories(novaCodeGen PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# LLVM backend (optional)
add_subdirectory(LLVM)

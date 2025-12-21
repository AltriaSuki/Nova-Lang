add_executable(nova
    nova.cpp
)

target_link_libraries(nova PRIVATE
    novaDriver
    novaCodeGen
    novaSema
    novaParse
    novaLex
    novaAST
    novaBasic
)

install(TARGETS nova RUNTIME DESTINATION bin)

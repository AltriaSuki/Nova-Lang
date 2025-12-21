add_executable(nova-repl
    repl.cpp
)

target_link_libraries(nova-repl PRIVATE
    novaLex
    novaBasic
)

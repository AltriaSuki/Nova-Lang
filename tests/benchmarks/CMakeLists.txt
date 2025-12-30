add_executable(nova-bench
    nova-bench.cpp
)

target_link_libraries(nova-bench PRIVATE
    novaLex
    novaBasic
)

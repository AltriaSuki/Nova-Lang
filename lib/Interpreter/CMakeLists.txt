add_library(novaInterpreter
    Value.cpp
    Environment.cpp
    Interpreter.cpp
)

target_link_libraries(novaInterpreter PUBLIC
    novaBasic
    novaAST
    novaRuntime
)

target_include_directories(novaInterpreter PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

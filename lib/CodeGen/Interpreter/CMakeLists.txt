add_library(novaInterpreter
    Interpreter.cpp
    Value.cpp
)
target_link_libraries(novaInterpreter PUBLIC novaBasic novaAST novaRuntime)
target_include_directories(novaInterpreter PUBLIC ${PROJECT_SOURCE_DIR}/include)

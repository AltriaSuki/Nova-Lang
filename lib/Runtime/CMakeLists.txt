add_library(novaRuntime
    Builtin.cpp
)
target_link_libraries(novaRuntime PUBLIC novaBasic)
target_include_directories(novaRuntime PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(novaDriver
    Driver.cpp
)

target_link_libraries(novaDriver PUBLIC
    novaBasic
)

target_include_directories(novaDriver PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

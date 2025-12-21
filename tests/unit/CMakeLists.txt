add_executable(novaTests
    LexerTest.cpp
    SourceLocationTest.cpp
)

target_link_libraries(novaTests PRIVATE
    novaLex
    novaBasic
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(novaTests)

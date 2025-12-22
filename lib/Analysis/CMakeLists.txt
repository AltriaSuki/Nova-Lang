add_library(novaAnalysis
    OwnershipAnalysis.cpp
    BorrowChecker.cpp
)
target_link_libraries(novaAnalysis PUBLIC novaAST novaSema novaBasic)
target_include_directories(novaAnalysis PUBLIC ${PROJECT_SOURCE_DIR}/include)

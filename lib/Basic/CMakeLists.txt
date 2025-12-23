add_library(novaBasic
    SourceLocation.cpp
    SourceManager.cpp
    IdentifierTable.cpp
    Diagnostic.cpp
    DiagnosticEngine.cpp
)

target_include_directories(novaBasic PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

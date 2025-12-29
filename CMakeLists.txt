cmake_minimum_required(VERSION 3.15)
project(Nova VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Work around some macOS toolchain layouts where libc++ headers live in the SDK.
if(APPLE)
    execute_process(
        COMMAND xcrun --sdk macosx --show-sdk-path
        OUTPUT_VARIABLE NOVA_MACOS_SDK_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(NOVA_MACOS_SDK_PATH AND EXISTS "${NOVA_MACOS_SDK_PATH}/usr/include/c++/v1/cstdint")
        include_directories(SYSTEM "${NOVA_MACOS_SDK_PATH}/usr/include/c++/v1")
        message(STATUS "Using libc++ headers from SDK: ${NOVA_MACOS_SDK_PATH}/usr/include/c++/v1")
    endif()
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Options
option(NOVA_BUILD_TESTS "Build tests" ON)
option(NOVA_ENABLE_LLVM "Enable LLVM backend (if available)" ON)

# Libraries
add_subdirectory(lib)

# Tools
add_subdirectory(tools)

# Tests
if(NOVA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Print configuration
message(STATUS "Nova Language Compiler")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build tests: ${NOVA_BUILD_TESTS}")
message(STATUS "  Enable LLVM: ${NOVA_ENABLE_LLVM}")

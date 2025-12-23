# LLVM Backend (optional, requires LLVM)

# Find LLVM package
find_package(LLVM CONFIG)

if(LLVM_FOUND)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

    add_library(novaLLVMCodeGen
        LLVMCodeGen.cpp
        LLVMTypeConverter.cpp
        LLVMExprEmitter.cpp
    )

    # LLVM definitions and includes
    separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
    target_compile_definitions(novaLLVMCodeGen PRIVATE ${LLVM_DEFINITIONS_LIST})
    target_include_directories(novaLLVMCodeGen PRIVATE ${LLVM_INCLUDE_DIRS})

    # Link required LLVM components
    llvm_map_components_to_libnames(llvm_libs
        Core
        Support
        IRReader
        BitWriter
        Passes
        native
    )

    target_link_libraries(novaLLVMCodeGen PUBLIC
        novaBasic
        novaAST
        novaCodeGen
        ${llvm_libs}
    )

    target_include_directories(novaLLVMCodeGen PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    )

    # Export that LLVM backend is available
    target_compile_definitions(novaLLVMCodeGen PUBLIC NOVA_HAS_LLVM_BACKEND)
else()
    message(STATUS "LLVM not found - LLVM backend will not be built")
    message(STATUS "To enable LLVM backend, install LLVM and set LLVM_DIR")
endif()

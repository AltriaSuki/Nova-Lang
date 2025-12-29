#!/bin/bash
set -e

echo "Adding missing components to Nova structure..."

# Create directories
mkdir -p include/nova/{IR,Transforms,Runtime,Analysis,CodeGen/LLVM,Interpreter}
mkdir -p lib/{IR,Transforms,Runtime,Analysis,CodeGen/LLVM,Interpreter}
mkdir -p stdlib/{core,collections,io,sync}

# Create CMakeLists files
if [[ ! -f lib/IR/CMakeLists.txt ]]; then
cat > lib/IR/CMakeLists.txt << 'EOF'
add_library(novaIR
    IR.cpp
    IRBuilder.cpp
    Module.cpp
)
target_link_libraries(novaIR PUBLIC novaBasic novaAST)
target_include_directories(novaIR PUBLIC ${PROJECT_SOURCE_DIR}/include)
EOF
fi

if [[ ! -f lib/Transforms/CMakeLists.txt ]]; then
cat > lib/Transforms/CMakeLists.txt << 'EOF'
add_library(novaTransforms
    Optimizer.cpp
    ConstantFolding.cpp
)
target_link_libraries(novaTransforms PUBLIC novaIR novaBasic)
target_include_directories(novaTransforms PUBLIC ${PROJECT_SOURCE_DIR}/include)
EOF
fi

if [[ ! -f lib/Analysis/CMakeLists.txt ]]; then
cat > lib/Analysis/CMakeLists.txt << 'EOF'
add_library(novaAnalysis
    OwnershipAnalysis.cpp
    BorrowChecker.cpp
)
target_link_libraries(novaAnalysis PUBLIC novaAST novaSema novaBasic)
target_include_directories(novaAnalysis PUBLIC ${PROJECT_SOURCE_DIR}/include)
EOF
fi

if [[ ! -f lib/Runtime/CMakeLists.txt ]]; then
cat > lib/Runtime/CMakeLists.txt << 'EOF'
add_library(novaRuntime
    Builtin.cpp
)
target_link_libraries(novaRuntime PUBLIC novaBasic)
target_include_directories(novaRuntime PUBLIC ${PROJECT_SOURCE_DIR}/include)
EOF
fi

# Interpreter is a top-level library in this repo (`lib/Interpreter/`), not under CodeGen.
if [[ ! -f lib/Interpreter/CMakeLists.txt ]]; then
cat > lib/Interpreter/CMakeLists.txt << 'EOF'
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
EOF
fi

# Create placeholder implementation files
touch lib/IR/{IR.cpp,IRBuilder.cpp,Module.cpp}
touch lib/Transforms/{Optimizer.cpp,ConstantFolding.cpp}
touch lib/Analysis/{OwnershipAnalysis.cpp,BorrowChecker.cpp}
touch lib/Runtime/Builtin.cpp
touch lib/Interpreter/{Interpreter.cpp,Value.cpp,Environment.cpp}

# Create header files
touch include/nova/IR/{IR.hpp,IRBuilder.hpp,Module.hpp}
touch include/nova/Transforms/Optimizer.hpp
touch include/nova/Analysis/{OwnershipAnalysis.hpp,BorrowChecker.hpp}
touch include/nova/Runtime/Builtin.hpp
touch include/nova/Interpreter/{Interpreter.hpp,Value.hpp,Environment.hpp}

# Create standard library files
touch stdlib/core/{option.nova,result.nova,prelude.nova}
touch stdlib/collections/{vec.nova,hashmap.nova}

echo "✓ Directory structure updated!"
echo ""
echo "Next steps:"
echo "1. Configure: cmake -S . -B build"
echo "2. Build: cmake --build build -j"

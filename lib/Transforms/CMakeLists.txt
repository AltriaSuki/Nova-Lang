add_library(novaTransforms
    Optimizer.cpp
    ConstantFolding.cpp
)
target_link_libraries(novaTransforms PUBLIC novaIR novaBasic)
target_include_directories(novaTransforms PUBLIC ${PROJECT_SOURCE_DIR}/include)

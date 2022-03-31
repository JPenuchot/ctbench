add_executable(cmake-doc-extractor cmake-doc-extractor.cpp)
target_link_libraries(cmake-doc-extractor PRIVATE ctbench-compile-opts)

find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs core support)
target_link_libraries(cmake-doc-extractor PRIVATE ${llvm_libs})

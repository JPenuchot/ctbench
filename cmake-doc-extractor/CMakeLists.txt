llvm_map_components_to_libnames(llvm_libs support)

add_executable(cmake-doc-extractor cmake-doc-extractor.cpp)
target_link_libraries(cmake-doc-extractor PRIVATE ctbench-compile-opts ${llvm_libs})

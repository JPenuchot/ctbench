add_executable(cmake-doc-extractor cmake-doc-extractor.cpp)

find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs core support)
#target_compile_options(cmake-doc-extractor PUBLIC -fno-rtti)
target_link_libraries(cmake-doc-extractor PUBLIC ${llvm_libs})

target_compile_options(
  cmake-doc-extractor
  PUBLIC -Wall
         -Wextra
         -Werror
         -Wnull-dereference
         -Wold-style-cast
         -Wdouble-promotion
         -Wshadow)

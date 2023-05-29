# Declaring the grapher library
file(GLOB_RECURSE GRAPHER_SOURCES lib/*.cpp)
add_library(grapher STATIC ${GRAPHER_SOURCES})

target_include_directories(grapher
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

llvm_map_components_to_libnames(llvm_libs support)

target_link_libraries(grapher
  PUBLIC
    ctbench-compile-opts
    nlohmann_json::nlohmann_json
    sciplot::sciplot
    fmt::fmt
    stdc++fs
    tbb
    ${llvm_libs})

target_compile_options(grapher PUBLIC -DJSON_NOEXCEPTION)

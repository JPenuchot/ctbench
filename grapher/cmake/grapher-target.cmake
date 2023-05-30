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
    ${llvm_libs}
    ctbench-compile-opts
    fmt::fmt
    nlohmann_json::nlohmann_json
    sciplot::sciplot
    stdc++fs
)

target_compile_options(grapher PUBLIC -DJSON_NOEXCEPTION)

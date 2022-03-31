find_package(nlohmann_json 3.9.1 REQUIRED)
find_package(sciplot REQUIRED)
find_package(LLVM REQUIRED CONFIG)

# Declaring the grapher library
file(GLOB_RECURSE GRAPHER_SOURCES lib/*.cpp)
add_library(grapher STATIC ${GRAPHER_SOURCES})
target_link_libraries(grapher PUBLIC ctbench-compile-opts)
target_include_directories(grapher PUBLIC include)

llvm_map_components_to_libnames(llvm_libs core support)

target_link_libraries(grapher PUBLIC nlohmann_json::nlohmann_json
                                     sciplot::sciplot stdc++fs tbb ${llvm_libs})

target_compile_options(grapher PUBLIC -DJSON_NOEXCEPTION)

# Adding executables

add_executable(ctbench-grapher-plot grapher-plot.cpp)
target_link_libraries(ctbench-grapher-plot PRIVATE grapher)

add_executable(ctbench-grapher-utils grapher-utils.cpp)
target_link_libraries(ctbench-grapher-utils PRIVATE grapher)

# Test targets

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

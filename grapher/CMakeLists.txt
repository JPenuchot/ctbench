# Creating the grapher library
file(GLOB_RECURSE GRAPHER_SOURCES lib/*.cpp)
add_library(grapher STATIC ${GRAPHER_SOURCES})

# Header libraries
target_include_directories(grapher PUBLIC include)
target_link_libraries(grapher PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(grapher PUBLIC ${sciplot_content_SOURCE_DIR})

# LLVM
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs core support)
target_compile_options(grapher PUBLIC -fno-rtti)
target_link_libraries(grapher PUBLIC ${llvm_libs})

# Old libstdc++ compatibility
target_link_libraries(grapher PUBLIC stdc++fs)

# Adding executable subdirectories
add_subdirectory(grapher-plot)
add_subdirectory(grapher-utils)

if(CTBENCH_ENABLE_TESTING)
  add_subdirectory(tests)
endif()

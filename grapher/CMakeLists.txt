cmake_minimum_required(VERSION 3.20)
project(grapher)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# External libraries
include(cmake/external_libraries.cmake)

# LLVM as a library
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs core support)

# Linking
include_directories("include/")
link_libraries(nlohmann_json::nlohmann_json)
link_libraries(${llvm_libs})
include_directories(${sciplot_content_SOURCE_DIR})

# Executable declarations

file(GLOB_RECURSE GRAPHER_SOURCES src/grapher/*.cpp)

add_executable(grapher src/grapher.cpp ${GRAPHER_SOURCES})
add_executable(grapher-utils src/grapher-utils.cpp ${GRAPHER_SOURCES})

cmake_minimum_required(VERSION 3.20)
project(grapher)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# External libraries
include(cmake/external_libraries.cmake)

# LLVM as a library
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs core support)

# Executable declaration
file(GLOB_RECURSE GRAPHER_SOURCES src/*.cpp)
add_executable(grapher ${GRAPHER_SOURCES})

# Linking
target_include_directories(grapher PRIVATE "include/")
target_link_libraries(grapher PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(grapher PRIVATE ${llvm_libs})
target_include_directories(grapher PRIVATE ${sciplot_content_SOURCE_DIR})

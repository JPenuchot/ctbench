cmake_minimum_required(VERSION 3.20)
project(grapher)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# Executable declaration
file(GLOB_RECURSE GRAPHER_SOURCES src/*.cpp)
add_executable(grapher ${GRAPHER_SOURCES})

# Internal library
target_include_directories(grapher PUBLIC "include/")

# LLVM as a library
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs core support)

target_include_directories(grapher PUBLIC ${LLVM_INCLUDE_DIRS})
target_link_libraries(grapher PUBLIC ${llvm_libs})

# External libraries
include(cmake/external_libraries.cmake)

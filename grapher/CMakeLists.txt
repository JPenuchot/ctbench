cmake_minimum_required(VERSION 3.20)
project(grapher)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# Internal library
include_directories("include/")

# LLVM as a library
find_package(LLVM REQUIRED CONFIG)
llvm_map_components_to_libnames(llvm_libs support)
include_directories(${LLVM_INCLUDE_DIRS})
link_libraries(${llvm_libs})
# LLVM libraries are compiled without RTTI
add_compile_options(-fno-rtti)

# External libraries
include(cmake/external_libraries.cmake)

# ==============================================================================
# Target declarations

file(GLOB_RECURSE GRAPHER_SOURCES src/grapher/*.cpp)

add_executable(grapher-compare
  src/grapher-compare.cpp ${GRAPHER_SOURCES})

add_executable(grapher-stacked-curves
  src/grapher-stacked-curves.cpp ${GRAPHER_SOURCES})

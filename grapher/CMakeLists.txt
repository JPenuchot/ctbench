cmake_minimum_required(VERSION 3.20)
project(grapher)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include_directories("include/")

include(cmake/external_libraries.cmake)

# ==============================================================================
# Target declarations

file(GLOB_RECURSE GRAPHER_SOURCES "src/*.cpp")

add_executable(grapher-compare
  grapher-compare.cpp ${GRAPHER_SOURCES})

add_executable(grapher-stacked-curves
  grapher-stacked-curves.cpp ${GRAPHER_SOURCES})

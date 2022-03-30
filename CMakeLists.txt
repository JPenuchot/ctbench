cmake_minimum_required(VERSION 3.21)
project(ctbench)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

if(ENABLE_CLANG_TIDY)
  set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)
endif()

add_compile_options(
  -Wall
  -Wextra
  -Werror
  -Wnull-dereference
  -Wold-style-cast
  -Wdouble-promotion
  -Wshadow)

# Docs
if(${PROJECT_IS_TOP_LEVEL})
  add_subdirectory(cmake-doc-extractor)
  include(cmake/docs.cmake)
endif()

add_subdirectory(ttw)
add_subdirectory(grapher)

export(
  EXPORT ctbench-targets
  NAMESPACE ctbench::
  FILE CtbenchTargets.cmake)

install(EXPORT ctbench-targets DESTINATION ctbench)

include(cmake/benchmarking.cmake)

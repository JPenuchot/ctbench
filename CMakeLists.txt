cmake_minimum_required(VERSION 3.21)
project(ctbench)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

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

set(GRAPHER_FOLDER ${CMAKE_BINARY_DIR}/grapher/ PARENT_SCOPE)
set(TTW_FOLDER ${CMAKE_BINARY_DIR}/ttw/ PARENT_SCOPE)

include(cmake/benchmarking.cmake)

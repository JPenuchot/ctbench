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

add_compile_options(-fno-rtti -fno-exceptions)

# Docs
if(${PROJECT_IS_TOP_LEVEL})
  add_subdirectory(cmake-doc-extractor)
  include(cmake/docs.cmake)
endif()

add_subdirectory(time-trace-wrapper)
add_subdirectory(grapher)

include(benchmarking.cmake)

set(CTBENCH_TIME_TRACE_WRAPPER_EXEC ${CTBENCH_TIME_TRACE_WRAPPER_EXEC} PARENT_SCOPE)

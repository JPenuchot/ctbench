cmake_minimum_required(VERSION 3.21)
project(ctbench)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# Project options
option(CTBENCH_ENABLE_TESTING "Enable testing for ctbench components." OFF)

option(CTBENCH_FETCH_LIBRARIES
       "Fetch libraries instead of using the ones installed locally." OFF)

add_compile_options(
  -Wall
  -Wextra
  -Werror
  -Wnull-dereference
  -Wold-style-cast
  -Wdouble-promotion
  -Wshadow)

add_subdirectory(time-trace-wrapper)

# Docs
if(${PROJECT_IS_TOP_LEVEL})
  add_subdirectory(cmake-doc-extractor)
  include(cmake/docs.cmake)
endif()

# Exporting time-trace-wrapper exec path to parent scope if needed
if(NOT ${PROJECT_IS_TOP_LEVEL})
  set(CTBENCH_TIME_TRACE_WRAPPER_EXEC
      ${CTBENCH_TIME_TRACE_WRAPPER_EXEC}
      PARENT_SCOPE)
endif()

add_subdirectory(grapher)

include(benchmarking.cmake)

cmake_minimum_required(VERSION 3.21)
project(ctbench)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

include(cmake/docs.cmake)

option(CTBENCH_ENABLE_TESTING "Enable testing for ctbench components." OFF)

add_subdirectory(time-trace-wrapper)

# Exporting time-trace-wrapper exec path to parent scope if needed
if(NOT ${PROJECT_IS_TOP_LEVEL})
  set(CTBENCH_TIME_TRACE_WRAPPER_EXEC
    ${CTBENCH_TIME_TRACE_WRAPPER_EXEC} PARENT_SCOPE)
endif()

add_subdirectory(grapher)

include(benchmarking.cmake)

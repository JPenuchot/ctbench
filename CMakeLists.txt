cmake_minimum_required(VERSION 3.18)
project(ctbench)

option(CTBENCH_ENABLE_TESTING "Enable testing for ctbench components." 0)

if(CTBENCH_ENABLE_TESTING)
  enable_testing()
endif(CTBENCH_ENABLE_TESTING)

add_subdirectory(time-trace-wrapper)
add_subdirectory(grapher)

include(benchmarking.cmake)
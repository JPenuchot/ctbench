# Simple standalone project for compile-time benchmarking using ctbench

# Usage:

# ```sh
# cmake --preset release
# cmake --build --preset release
# ```

# CMake presets use clang/clang++ by default with time-trace enabled.

cmake_minimum_required(VERSION 3.25)
project(example-project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(ctbench REQUIRED)

add_compile_options(
  -Wall
  -Wextra
  -Werror
  -Wnull-dereference
  -Wold-style-cast
  -Wdouble-promotion
  -Wshadow)

if(ENABLE_TIME_TRACE)
  add_compile_options(-ftime-trace -ftime-trace-granularity=1
                      -fconstexpr-steps=2147483647 -fbracket-depth=2147483647)
endif()

set(BENCHMARK_START 1 CACHE STRING "Benchmark size minimum")
set(BENCHMARK_STOP 16 CACHE STRING "Benchmark size maximum")
set(BENCHMARK_STEP 1 CACHE STRING "Benchmark size step")
set(BENCHMARK_ITERATIONS 1 CACHE STRING "Number of samples per size")

ctbench_add_benchmark(
  variadic_sum.expansion variadic_sum/expansion.cpp ${BENCHMARK_START}
  ${BENCHMARK_STOP} ${BENCHMARK_STEP} ${BENCHMARK_ITERATIONS})

ctbench_add_benchmark(
  variadic_sum.recursive variadic_sum/recursive.cpp ${BENCHMARK_START}
  ${BENCHMARK_STOP} ${BENCHMARK_STEP} ${BENCHMARK_ITERATIONS})

ctbench_add_graph(variadic_sum-compare-graph compare-all.json
                  variadic_sum.expansion variadic_sum.recursive)

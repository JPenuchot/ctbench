### ============================================================================
### benchmarking.cmake
###
### Functions for automated benchmark declarations.
### ============================================================================

# This is mostly because Boost Preprocessor is widely used across benchmarks
find_package(Boost REQUIRED)

# graph-all dummy target
add_custom_target(ctbench-graph-all)

## =============================================================================
## Internal use only.
##
## Creates a library target for a file
## and extracts the compilation time trace file.
##
## - target_name: Name of the benchmark target
## - output: Time trace output path
## - file: Source file
## - size: Sets BENCHMARK_SIZE define (can be something else than a number)
##   See: https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_DEFINITIONS.html

function(_ctbench_internal_add_compile_benchmark target_name output source size)

  add_library(${target_name} OBJECT EXCLUDE_FROM_ALL ${source})
  target_include_directories(${target_name} PUBLIC "../include")

  # Setting time-trace-wrapper as a compiler launcher
  set_target_properties(${target_name}
    PROPERTIES
      CXX_COMPILER_LAUNCHER
        "${CTBENCH_TIME_TRACE_WRAPPER_EXEC};${output}")

  # Adding dependency because CMake won't
  add_dependencies(${target_name} time-trace-wrapper)

  # Pass benchmark size
  set_target_properties(${target_name}
    PROPERTIES
      COMPILE_DEFINITIONS "BENCHMARK_SIZE=${size}")

  # Boost Preprocessor
  target_include_directories(${target_name} PUBLIC Boost_INCLUDE_DIRS)

endfunction(_ctbench_internal_add_compile_benchmark)


## =============================================================================
## Add a benchmark for a given source, with a given size range.
##
## - name: Name of benchmark range
## - source: Source file
## - begin: Size iteration begin
## - end: Size iteration end
## - step: Size iteration step
## - target_name_out: Output variable for target name

function(ctbench_add_benchmark name source begin end step)

  set(range_spec "${begin}.${end}.${step}")
  set(range_target_name ${name})
  set(folder_name ${name})

  add_custom_target(${range_target_name})
  set(${target_name_out} ${range_target_name})

  foreach(size RANGE ${begin} ${end} ${step})

    set(range_subtarget_name "_${name}-${size}")

    _ctbench_internal_add_compile_benchmark(
      ${range_subtarget_name}
      "${folder_name}/${size}.json"
      "${source}"
      "${size}")

    add_dependencies(${range_target_name} ${range_subtarget_name})

  endforeach()

endfunction(ctbench_add_benchmark)

## =============================================================================
## Adds a graph target for a set of benchmarks,
## and adds the target to ctbench-graph-all.
##
## - graph_name: target name and output folder
## - benchmarks: List of benchmark names

function(ctbench_add_graphs graph_name)

  add_custom_target(${graph_name}
    COMMAND grapher ${graph_name} ${ARGN}
    DEPENDS ${ARGN})

  add_dependencies(ctbench-graph-all graph_name)

endfunction(ctbench_add_graphs)
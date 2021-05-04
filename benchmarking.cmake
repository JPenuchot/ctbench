### ============================================================================
### benchmarking.cmake
###
### Functions for automated benchmark declarations.
### ============================================================================

# This is mostly because Boost Preprocessor is widely used across benchmarks
find_package(Boost REQUIRED)

# graph-all dummy target
add_custom_target(ctbench-graph-all)

include(cmake/internal.cmake)

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

  add_dependencies(ctbench-graph-all ${graph_name})

endfunction(ctbench_add_graphs)
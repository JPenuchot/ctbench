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
## - name: Name of benchmark
## - source: Source file
## - begin: Size iteration begin
## - end: Size iteration end
## - step: Size iteration step
## - iterations: Number of benchmark iterations

function(ctbench_add_benchmark name source begin end step iterations)
  # Setting names
  add_custom_target(${name})

  foreach(iteration RANGE ${iterations})
    foreach(size RANGE ${begin} ${end} ${step})
      # Subtargets aren't meant to be compiled by end-users
      set(subtarget_name "_${name}-size_${size}-it_${iteration}")

      _ctbench_internal_add_compile_benchmark(
        ${subtarget_name}
        "${name}/${size}/${iteration}.json"
        "${source}"
        "${size}")

      add_dependencies(${name} ${subtarget_name})
    endforeach()
  endforeach()

endfunction(ctbench_add_benchmark)

## =============================================================================
## Adds a graph target for a set of benchmarks,
## and adds the target to ctbench-graph-all.
##
## - category: Name of the category. This is also the name of the graph target,
##   and the folder where the graphs will be saved.
## - benchmarks: List of benchmark names

function(ctbench_graph_category category)
  add_custom_target(${category}
    COMMAND grapher ${category} ${ARGN}
    DEPENDS ${ARGN})
  add_dependencies(ctbench-graph-all ${category})
endfunction(ctbench_graph_category)

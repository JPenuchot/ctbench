## =============================================================================
#! # `benchmarking.cmake`
#!
#! Functions for automated benchmark declarations.
## =============================================================================

# graph-all dummy target
add_custom_target(ctbench-graph-all)

include(cmake/internal.cmake)

## =============================================================================
#!
#! ## `ctbench_add_benchmark`
#!
#! Add a benchmark for a given source, with a given size range.
#!
#! - `name`: Name of benchmark
#! - `source`: Source file
#! - `begin, end, step`: Iteration parameters
#! - `iterations`: Number of benchmark iterations
#!

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
        "-DBENCHMARK_SIZE=${size}")

      add_dependencies(${name} ${subtarget_name})
    endforeach()
  endforeach()

endfunction(ctbench_add_benchmark)

## =============================================================================
#!
#! ## `ctbench_add_custom_benchmark`
#!
#! Add a benchmark for a given source with a given size range
#! using a custom compile options generator.
#!
#! - `name`: Name of benchmark
#! - `source`: Source file
#! - `begin, end, step`: Iteration parameters
#! - `iterations`: Number of benchmark iterations for a given size
#! - `generator`: Compile option generator. Takes a size and an output
#!                variable name as parameters.
#!

function(ctbench_add_custom_benchmark
  name
  source
  begin end step
  iterations
  generator)

  # Setting names
  add_custom_target(${name})

  foreach(iteration RANGE 1 ${iterations})
    foreach(size RANGE ${begin} ${end} ${step})
      # Subtargets aren't meant to be compiled by end-users
      set(subtarget_name "_${name}-size_${size}-it_${iteration}")

      cmake_language(CALL ${generator} ${size} ctbench_options_output)

      _ctbench_internal_add_compile_benchmark(
        ${subtarget_name}
        "${name}/${size}/${iteration}.json"
        "${source}"
        "${ctbench_options_output}")

      add_dependencies(${name} ${subtarget_name})
    endforeach()
  endforeach()

endfunction(ctbench_add_custom_benchmark)

## =============================================================================
#!
#! ## `ctbench_add_graph`
#!
#! Adds a graph target for a set of benchmarks,
#! and adds the target to ctbench-graph-all.
#!
#! - `category`: Name of the category. This is also the name of the graph
#!               target, and the folder where the graphs will be saved.
#! - `benchmarks`: List of benchmark names
#!

function(ctbench_add_graph category plotter config)
  add_custom_target(${category}
    COMMAND grapher-plot
      --output=${category} --plotter=${plotter} --config=${config}
      ${ARGN}
    DEPENDS ${config} ${ARGN})
  add_dependencies(ctbench-graph-all ${category})
endfunction(ctbench_add_graph)

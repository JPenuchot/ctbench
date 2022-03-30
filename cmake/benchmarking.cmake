#! # CMake documentation

## =============================================================================
#@
#@ ## Internal API
#@
## =============================================================================

# Setting executable/target prefix.
# ctbench_FOUND being false means ctbench was imported inside of the tree.
# ctbench_FOUND being true means ctbench found through find_package.
if(NOT ctbench_FOUND)
  set(GRAPHER_PREFIX
      ${CMAKE_CURRENT_BINARY_DIR}/grapher/
      PARENT_SCOPE)
  set(TTW_PREFIX
      ${CMAKE_CURRENT_BINARY_DIR}/ttw/
      PARENT_SCOPE)
else()
  set(GRAPHER_PREFIX
      ctbench::
      PARENT_SCOPE)
  set(TTW_PREFIX
      ctbench::
      PARENT_SCOPE)
endif()

## =============================================================================
#@
#@ _ctbench_internal_add_compile_benchmark
#@
#@ Creates a library target for a file and extracts the compilation time trace
#@ file.
#@
#@ - `target_name`: Name of the benchmark target
#@ - `output`: Time trace output path
#@ - `source`: Source file
#@ - `options`: Options passed to the compiler

function(_ctbench_internal_add_compile_benchmark target_name output source
         options)

  add_library(${target_name} OBJECT EXCLUDE_FROM_ALL ${source})
  target_include_directories(${target_name} PUBLIC "../include")

  # Setting ctbench-ttw as a compiler launcher
  set_target_properties(
    ${target_name} PROPERTIES CXX_COMPILER_LAUNCHER
                              "${TTW_PREFIX}ctbench-ttw;${output}")

  if(NOT ctbench_FOUND)
    add_dependencies(${target_name} ctbench-ttw)
  endif()

  # Pass benchmark size
  target_compile_options(${target_name} PRIVATE ${options})

endfunction(_ctbench_internal_add_compile_benchmark)

## =============================================================================
#@
#@ ## Public API
#@
## =============================================================================

## =============================================================================
#!
#! ### ctbench-graph-all target
#!
#! `ctbench-graph-all` is a ctbench-provided target to generate all graphs
#! at once.

add_custom_target(ctbench-graph-all)

#!
#! --

## =============================================================================
#!
#! ### ctbench_add_benchmark(name source begin end step samples)
#!
#! Add a benchmark for a given source, with a given size range.
#!
#! - `name`: Name of benchmark
#! - `source`: Source file
#! - `begin, end, step`: Iteration parameters
#! - `samples`: Number of samples per iteration

function(
  ctbench_add_benchmark
  name
  source
  begin
  end
  step
  samples)
  # Setting names
  add_custom_target(${name})

  foreach(iteration RANGE ${samples})
    foreach(size RANGE ${begin} ${end} ${step})
      # Subtargets aren't meant to be compiled by end-users
      set(subtarget_name "_${name}-size_${size}-it_${iteration}")

      _ctbench_internal_add_compile_benchmark(
        ${subtarget_name} "${name}/${size}/${iteration}.json" "${source}"
        "-DBENCHMARK_SIZE=${size}")

      target_compile_options(${subtarget_name}
                             PRIVATE -ftime-trace-granularity=1)

      add_dependencies(${name} ${subtarget_name})
    endforeach()
  endforeach()

endfunction(ctbench_add_benchmark)

#!
#! --

## =============================================================================
#!
#! ### ctbench_add_custom_benchmark(name source begin end step iterations generator)
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

function(
  ctbench_add_custom_benchmark
  name
  source
  begin
  end
  step
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
        ${subtarget_name} "${name}/${size}/${iteration}.json" "${source}"
        "${ctbench_options_output}")

      add_dependencies(${name} ${subtarget_name})
    endforeach()
  endforeach()

endfunction(ctbench_add_custom_benchmark)

#!
#! --

## =============================================================================
#!
#! ### ctbench_add_graph(category config benchmarks...)
#!
#! Adds a graph target for a set of benchmarks,
#! and adds the target to ctbench-graph-all.
#!
#! - `category`: Name of the category. This is also the name of the graph
#!               target, and the folder where the graphs will be saved.
#! - `config`: Config file for plotting
#! - `benchmarks...`: List of benchmark names

function(ctbench_add_graph category config)
  set(config_path ${CMAKE_CURRENT_SOURCE_DIR}/${config})
  add_custom_target(
    ${category}
    COMMAND ${GRAPHER_PREFIX}ctbench-grapher-plot --output=${category}
            --config=${config_path} ${ARGN}
    DEPENDS ${config_path} ${ARGN}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  add_dependencies(ctbench-graph-all ${category})

  if(NOT ctbench_FOUND)
    add_dependencies(${category} ctbench-grapher-plot)
  endif()
endfunction(ctbench_add_graph)

#!
#! --

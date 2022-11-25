#! # CMake documentation

## =============================================================================
#@
#@ ## Internal API
#@
## =============================================================================

#@ Sets ctbench executable/target prefixes for FetchContent support.
function(_ctbench_set_prefixes)
  # ctbench_FOUND being false means ctbench was imported inside of the tree.
  # ctbench_FOUND being true means ctbench found through find_package.
  if(ctbench_FOUND)
    set(GRAPHER_PREFIX
        ctbench::
        CACHE STRING "Prefix for grapher executables")

        # ttw is a special case as it is used as a compiler launcher.
    # Therefore we can't use the imported executable target,
    # and have to provide an executable name assuming ttw can be found in PATH.
    set(TTW_PREFIX
        ""
        CACHE STRING "Prefix for TTW executable")
  else()
    message("not cool")
    set(GRAPHER_PREFIX
        ${CMAKE_CURRENT_BINARY_DIR}/grapher/
        CACHE STRING "Prefix for grapher executables")
    set(TTW_PREFIX
        ${CMAKE_CURRENT_BINARY_DIR}/ttw/
        CACHE STRING "Prefix for TTW executable")
  endif()
endfunction()

## =============================================================================
#@
#@ _ctbench_internal_add_compile_benchmark
#@
#@ Creates a library target for a file, and runs the compiler using
#@ time-trace-wrapper.
#@
#@ - `target_name`: Name of the benchmark target
#@ - `output`: Time trace output path
#@ - `source`: Source file
#@ - `options`: Options passed to the compiler

function(_ctbench_internal_add_compile_benchmark target_name output source
         options)
  _ctbench_set_prefixes()
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
#! Add a benchmark for a given source, with a given size range. Please note that
#! ctbench_add_benchmark does *not* add -ftime-trace flags. If not present,
#! the compiler execution time will be measured by the compiler execution
#! wrapper, and only this data will be reported.
#!
#! If you need Clang's time-trace data, please specify it manually,
#! and do not forget to set -ftime-trace-granularity if needed.
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
  _ctbench_set_prefixes()
  # Setting names
  add_custom_target(${name})

  foreach(iteration RANGE ${samples})
    foreach(size RANGE ${begin} ${end} ${step})
      # Subtargets aren't meant to be compiled by end-users
      set(subtarget_name "_${name}-size_${size}-it_${iteration}")

      _ctbench_internal_add_compile_benchmark(
        ${subtarget_name} "${name}/${size}/${iteration}.json" "${source}"
        "-DBENCHMARK_SIZE=${size}")

      add_dependencies(${name} ${subtarget_name})
    endforeach()
  endforeach()

endfunction(ctbench_add_benchmark)

#!
#! --

## =============================================================================
#!
#! ### ctbench_add_custom_benchmark(name source begin end step samples callback)
#!
#! Add a benchmark for a given source with a given size range
#! using a custom compile options generator.
#!
#! - `name`: Name of benchmark
#! - `source`: Source file
#! - `begin, end, step`: Iteration parameters
#! - `samples`: Number of benchmark samples for a given size
#! - `callback`: Callback function that is called for each benchmark iteration
#!   target. For each target, ctbench will pass it the size and target name.

function(
  ctbench_add_custom_benchmark
  name
  source
  begin
  end
  step
  samples
  generator)
  _ctbench_set_prefixes()

  # Setting names
  add_custom_target(${name})

  foreach(iteration RANGE 1 ${samples})
    foreach(size RANGE ${begin} ${end} ${step})
      # Subtargets aren't meant to be compiled by end-users
      set(subtarget_name "_${name}-size_${size}-it_${iteration}")

      cmake_language(CALL ${generator} ${size} ${subtarget_name})

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
  _ctbench_set_prefixes()

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

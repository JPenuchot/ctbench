#! # CMake API reference

## =============================================================================
#@
#@ ## Internal API
#@
## =============================================================================

## =============================================================================
#@
#@ ### _ctbench_internal_add_compile_benchmark
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
  add_library(${target_name} OBJECT EXCLUDE_FROM_ALL ${source})
  target_include_directories(${target_name} PUBLIC "../include")

  # Setting ctbench-compiler-launcher as a compiler launcher to handle
  # time-trace file retrieval and compiler overriding
  set_target_properties(
    ${target_name} PROPERTIES CXX_COMPILER_LAUNCHER
                              "$<TARGET_FILE:ctbench::ctbench-compiler-launcher>;${output}")

  # Pass benchmark size
  target_compile_options(${target_name} PRIVATE ${options})

  # Compiler overriding
  if(_CTBENCH_OVERRIDE_COMPILER)
    target_compile_options(${target_name} PRIVATE
      --override-compiler=${_CTBENCH_OVERRIDE_COMPILER})
  endif()
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
#! Adds a benchmark for a given source, with a given size range.
#! Please note that ctbench_add_benchmark does *not* add -ftime-trace flags.
#! If not present, the compiler execution time will be measured by the
#! compiler execution wrapper, and only this data will be reported.
#!
#! Each benchmark target declared by the function will be declared with
#! BENCHMARK_SIZE defined to its instantiation size.
#!
#! For example:
#!
#! ```cmake
#! ctbench_add_benchmark(some_bench some_bench.cpp 1 16 2 5)
#! ```
#!
#! This code creates targets with BENCHMARK_SIZE set from 1 to 16 with
#! a step of 2, with 5 repetitions for each benchmark instantiation size.
#! `some_bench.cpp` will be compiled 40 times in total.
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
  add_custom_target(${name})
  # Setting names
  foreach(size RANGE ${begin} ${end} ${step})
    foreach(iteration RANGE 1 ${samples})
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
#! ### ctbench_add_benchmark_for_range(name source range_var samples)
#!
#! Add a benchmark for a given source, with a given size range. Please note that
#! ctbench_add_benchmark_for_range does *not* add -ftime-trace flags.
#! If not present, the compiler execution time will be measured
#! by the compiler execution wrapper, and only this data will be reported.
#!
#! Each benchmark target declared by the function will be declared with
#! BENCHMARK_SIZE defined to its instantiation size.
#!
#! For example:
#!
#! ```cmake
#! set(small_range 1 16 2)
#! ctbench_add_benchmark_for_range(some_bench some_bench.cpp small_range 5)
#! ```
#!
#! This code creates targets with BENCHMARK_SIZE set from 1 to 16 with
#! a step of 2, with 5 repetitions for each benchmark instantiation size.
#! `some_bench.cpp` will be compiled 40 times in total.
#!
#! If you need Clang's time-trace data, please specify it manually,
#! and do not forget to set -ftime-trace-granularity if needed.
#!
#! - `name`: Name of benchmark
#! - `source`: Source file
#! - `range_var`: Variable name of a list containing begin, end, and step
#!   size parameters.
#! - `samples`: Number of samples per iteration

function(
  ctbench_add_benchmark_for_range
  name
  source
  range_var
  samples)
  list(GET ${range_var} 0 begin)
  list(GET ${range_var} 1 end)
  list(GET ${range_var} 2 step)
  ctbench_add_benchmark(${name} ${source} ${begin} ${end} ${step} ${samples})
endfunction(ctbench_add_benchmark_for_range)

#!
#! --

## =============================================================================
#!
#! ### ctbench_add_benchmark_for_size_list(name source begin end step samples)
#!
#! Add a benchmark for a given source, for a list of bench sizes. Please note
#! that ctbench_add_benchmark_for_size_list does *not* add -ftime-trace flags. If not
#! present, the compiler execution time will be measured by the compiler
#! execution wrapper, and only this data will be reported.
#!
#! If you need Clang's time-trace data, please specify it manually,
#! and do not forget to set -ftime-trace-granularity if needed.
#!
#! - `name`: Name of benchmark
#! - `source`: Source file
#! - `size_list`: Iteration size list variable name
#! - `samples`: Number of samples per iteration

function(
  ctbench_add_benchmark_for_size_list
  name
  source
  size_list
  samples)
  # Setting names
  add_custom_target(${name})

  foreach(iteration RANGE 1 ${samples})
    foreach(size ${size_list})
      # Subtargets aren't meant to be compiled by end-users
      set(subtarget_name "_${name}-size_${size}-it_${iteration}")

      _ctbench_internal_add_compile_benchmark(
        ${subtarget_name} "${name}/${size}/${iteration}.json" "${source}"
        "-DBENCHMARK_SIZE=${size}")

      add_dependencies(${name} ${subtarget_name})
    endforeach()
  endforeach()
endfunction(ctbench_add_benchmark_for_size_list)

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
  set(config_path ${CMAKE_CURRENT_SOURCE_DIR}/${config})
  add_custom_target(
    ${category}
    COMMAND ctbench::ctbench-grapher-plot --output=${category}
            --config=${config_path} ${ARGN}
    DEPENDS ${config_path} ${ARGN}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  add_dependencies(ctbench-graph-all ${category})
endfunction(ctbench_add_graph)

#!
#! --

## =============================================================================
#!
#! ### ctbench_set_compiler(compiler)
#!
#! Sets the compiler for the next benchmarks.
#! Use `ctbench_unset_compiler()` to revert back to the default compiler.

function(ctbench_set_compiler compiler)
  set(_CTBENCH_OVERRIDE_COMPILER ${compiler} CACHE INTERNAL "Compiler override variable" FORCE)
endfunction()

#!
#! --

## =============================================================================
#!
#! ### ctbench_unset_compiler()
#!
#! Reverts compiler for the next benchmarks back to default.
#! (see `ctbench_set_compiler(compiler)`)

function(ctbench_unset_compiler)
  unset(_CTBENCH_OVERRIDE_COMPILER CACHE)
endfunction()

#!
#! --

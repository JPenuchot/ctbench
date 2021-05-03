### ============================================================================
### benchmarking.cmake
###
### Functions for automated benchmark declarations.
### ============================================================================

# This is mostly because Boost Preprocessor is widely used across benchmarks
find_package(Boost REQUIRED)

## =============================================================================
## Creates a library target for a file
## and extracts the compilation time trace file.
## - target_name: Name of the benchmark target
## - output: Time trace output path
## - file: Source file
## - size: Sets BENCHMARK_SIZE define (can be something else than a number)
##   See: https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_DEFINITIONS.html

function(add_compile_benchmark target_name output source size)
  add_library(${target_name} OBJECT EXCLUDE_FROM_ALL ${source})
  target_include_directories(${target_name} PUBLIC "../include")

  # Setting time-trace-wrapper as a compiler launcher
  set_target_properties(${target_name}
    PROPERTIES
      CXX_COMPILER_LAUNCHER
        "ctbench/time-trace-wrapper/time-trace-wrapper;${output}")

  # Pass benchmark size
  set_target_properties(${target_name}
    PROPERTIES
      COMPILE_DEFINITIONS "BENCHMARK_SIZE=${size}")

  # Boost Preprocessor
  target_include_directories(${target_name} PUBLIC Boost_INCLUDE_DIRS)
endfunction(add_compile_benchmark)


## =============================================================================
## Add a benchmark range for a given source.
## - name: Name of the benchmark
## - source: Source file
## - begin: Size iteration begin
## - end: Size iteration end
## - step: Size iteration step

function(add_benchmark_range name source begin end step)
  add_custom_target("${name}-all")

  foreach(size RANGE ${begin} ${end} ${step})
    add_compile_benchmark(
      "_${name}-${size}"
      "${name}/${size}.json"
      "${source}"
      "${size}")
    add_dependencies("${name}-all" "_${name}-${size}")
  endforeach()

  set(GRAPH_TARGETS "${GRAPH_TARGETS};${name}")
endfunction(add_benchmark_range)


## =============================================================================
## Add a whole folder to benchmark targets
## - source: Input path for sources
## - dest: Output path for time trace reports

function(add_benchmark_folder sources dest)
  file(GLOB_RECURSE benchmark_sources "${sources}/*")
  foreach(benchmark_source ${benchmark_sources})
    # Generating benchmark target name and output filename

    # TODO: Making stems actually readable
    # https://cmake.org/cmake/help/latest/command/cmake_path.html
    string(MD5 ${benchmark_source} stem)
    string(REPLACE ".cpp" ".json" benchmark_output ${benchmark_source})

    # Adding benchmark target
    add_compile_benchmark(
      "benchmark_${stem}"
      ${benchmark_output}
      ${benchmark_source}
      0)
  endforeach(benchmark_source)
endfunction(add_benchmark_folder)

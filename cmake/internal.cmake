# Creates a library target for a file and extracts the compilation time trace
# file.
#
# * target_name: Name of the benchmark target
# * output: Time trace output path
# * file: Source file
# * size: Sets BENCHMARK_SIZE define (can be something else than a number)
# * options: Options passed to the compiler

function(_ctbench_internal_add_compile_benchmark target_name output source
         options)

  add_library(${target_name} OBJECT EXCLUDE_FROM_ALL ${source})
  target_include_directories(${target_name} PUBLIC "../include")

  # Setting time-trace-wrapper as a compiler launcher
  set_target_properties(
    ${target_name} PROPERTIES CXX_COMPILER_LAUNCHER
                              "${CTBENCH_TIME_TRACE_WRAPPER_EXEC};${output}")

  # Adding dependency because CMake won't
  add_dependencies(${target_name} time-trace-wrapper)

  # Pass benchmark size
  target_compile_options(${target_name} PRIVATE -ftime-trace-granularity=1
                                                ${options})

  # Boost Preprocessor
  target_include_directories(${target_name} PUBLIC Boost_INCLUDE_DIRS)

endfunction(_ctbench_internal_add_compile_benchmark)

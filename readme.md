# ctbench

Set of tools for comprehensive benchmarks based on clang's time-trace feature.

`ctbench` allows you to declare and generate compile-time benchmark batches for
given ranges, run them, aggregate time-trace reports, and plot them.
This allows you to get more comprehensive data than just compiler runtime
or single-size flame graphs.

`ctbench` was made to fit the needs of **scientific data collection
and analysis**, thus it is not a one-shot profiler, but a set of tools that
allow you to get **reproductible data** from user-defined compile-time
benchmarks.

It has two main components: a CMake boilerplate library and a C++ plotting
toolset.

The CMake library contains all the boilerplate code to define benchmark targets
compatible with the C++ plotting toolset called `grapher`.

## How-to

You can integrate `ctbench` by adding this into your CMake listfiles:

```cmake
include(FetchContent)
FetchContent_Declare(
  ctbench
  GIT_REPOSITORY https://github.com/jpenuchot/ctbench.git main
  GIT_SHALLOW)
FetchContent_MakeAvailable(ctbench)
```

The [Rule of Cheese](https://github.com/JPenuchot/rule-of-cheese) project can be
used as an example of how to use `ctbench`. It is the project that gave birth to
this tool and is maintained as `ctbench` evolves.

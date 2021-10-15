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

[Rule of Cheese](https://github.com/jpenuchot/rule-of-cheese) can be used as a
reference project for using ctbench.

## Using ctbench

### Integrating ctbench in your project

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

### Declaring your first benchmark case target

A benchmark case is represented by a C++ file. It will be "instanciated", ie.
compiled with `BENCHMARK_SIZE` defined to values in a range that you provide.

`BENCHMARK_SIZE` is intended to be used by the preprocessor to generate a
benchmark instance of the desired size:

```cpp
#include <boost/preprocessor/repetition/repeat.hpp>

// First we generate foo<int>().
// foo<int>() uses C++20 requirements to dispatch function calls accross 16
// of its instances, according to the value of its integer template parameter.

#define FOO_MAX 16

#define DECL(z, i, nope)                                                       \
  template <int N>                                                             \
  requires(N % FOO_MAX == i) constexpr int foo() { return N * i; }

BOOST_PP_REPEAT(BENCHMARK_SIZE, DECL, FOO_MAX);
#undef DECL

// Now we generate the sum() function for instanciation

int sum() {
  int i;

#define CALL(z, n, nop) i += foo<n>();
  BOOST_PP_REPEAT(BENCHMARK_SIZE, CALL, i);
#undef CALL
  return i;
}
```

Then you can declare a benchmark case target in CMake with the following:

```cmake
ctbench_add_benchmark(function_selection.requires # Benchmark case name
  function_selection-requires.cpp                 # Benchmark case file
  1                                               # Range begin
  32                                              # Range end
  1                                               # Range step
  10)                                             # Iterations per size
```

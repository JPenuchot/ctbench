# `ctbench`

Set of tools for comprehensive benchmarks based on clang's time-trace feature.

`ctbench` allows you to declare and generate compile-time benchmark batches for
given ranges, run them, aggregate time-trace reports, and plot them.
This allows you to get more comprehensive data than just compiler runtime
or single-size flame graphs.

Also, `ctbench` was made with the intents of **integration** and **reuse**.

## Vocabulary

A `benchmark` is a matrix of `entries` of dimensions M * N, where M is the
number of different entry sizes (ie. the size parameter given for a benchmark
case), and N is the number of iterations. All sizes have the same number of
iterations.

A `category` of benchmarks is a group of benchmarks.
Benchmark plots for a given category are all at the same scale.

## How-to

Add the following to your `CMakeLists.txt` file:

```cmake
include(FetchContent)
FetchContent_Declare(
  ctbench
  GIT_REPOSITORY https://github.com/jpenuchot/ctbench.git
  GIT_TAG        origin/main)
FetchContent_MakeAvailable(ctbench)
```

## Examples

See my [Rule of Cheese](https://github.com/JPenuchot/rule-of-cheese) project.

## Notes

The project is more of a WIP.
Some of these features will be added in the future:

- Options for `grapher` to select which features to put in the graph
- Multiple benchmark iterations

# ctbench example

## Benchmark execution

ctbench must be installed prior to running this example.
Please refer to the main readme file for installation instructions.

To compile the example, build the `variadic_sum-compare-graph`
with the following commands:

```sh
cmake --preset release
cmake --build --preset release
```

The output graphs should be located under
`build/release/variadic_sum-compare-graph`.

## Presentation

This example covers a short yet practical example of ctbench usage. We want to
calculate the sum of a series of integers known at compile-time, using a type
template to store unsigned integer values at compile-time.

We will be comparing the compile-time performance of two implementations:
- one based on a recursive function template,
- and one based on C++11 parameter pack expansion.

First we need to include `utility` to instantiate our benchmark according to the
size parameter using `std::make_index_sequence`, and define the compile-time
container type for an unsigned integer:

```cpp
#include <utility>

/// Compile-time std::size_t
template <std::size_t N> struct ct_uint_t {
  static constexpr std::size_t value = N;
};
```

The first version of the metaprogram is based on a recursive template function:

```cpp
/// Recursive compile-time sum implementation
template<typename ... Ts> constexpr auto sum();

template <> constexpr auto sum() { return ct_uint_t<0>{}; }
template <typename T> constexpr auto sum(T const &) { return T{}; }

template <typename T, typename... Ts>
constexpr auto sum(T const &, Ts const &...tl) {
  return ct_uint_t<T::value + decltype(sum(tl...))::value>{};
}
```

And the other version relies on C++11 parameter pack expansion:

```cpp
/// Expansion compile-time sum implementation
template<typename ... Ts> constexpr auto sum();

template <> constexpr auto sum() { return ct_uint_t<0>{}; }

template <typename... Ts> constexpr auto sum(Ts const &...) {
  return ct_uint_t<(Ts::value + ... + 0)>{};
}
```

Both versions share the same interface, and thus the same driver code as well.
The driver code takes care of scaling the benchmark according to
`BENCHMARK_SIZE`, which is defined by **ctbench** through the CMake API:

```cpp
// Driver code

template <typename = void> constexpr auto foo() {
  return []<std::size_t... Is>(std::index_sequence<Is...>) {
    return sum(ct_uint_t<Is>{}...);
  }
  (std::make_index_sequence<BENCHMARK_SIZE>{});
}

[[maybe_unused]] constexpr std::size_t result =
    decltype(foo())::value;
```

The CMake code needed to run the benchmarks is the following:

```cmake
ctbench_add_benchmark(
  variadic_sum.expansion variadic_sum/expansion.cpp ${BENCHMARK_START}
  ${BENCHMARK_STOP} ${BENCHMARK_STEP} ${BENCHMARK_ITERATIONS})

ctbench_add_benchmark(
  variadic_sum.recursive variadic_sum/recursive.cpp ${BENCHMARK_START}
  ${BENCHMARK_STOP} ${BENCHMARK_STEP} ${BENCHMARK_ITERATIONS})
```

Then a graph target can be declared:

```cmake
ctbench_add_graph(variadic_sum-compare-graph compare-all.json
                  variadic_sum.expansion variadic_sum.recursive)
```

with `compare-all.json` containing the following:

```json
{
  "plotter": "compare_by",
  "legend_title": "Timings",
  "x_label": "Benchmark size factor",
  "y_label": "Time (µs)",
  "draw_average": true,
  "demangle": false,
  "draw_points": false,
  "width": 800,
  "height": 400,
  "key_ptrs": ["/name", "/args/detail"],
  "value_ptr": "/dur",
  "plot_file_extensions": [".svg"]
}
```

This configuration file uses the `compare_by` plotter to generate one plot for
each pair of elements designated by the JSON pointers in `key_ptrs`, namely
`/name` and `/args/detail`. The first pointer designates the LLVM timer name,
and the second *may* refer to metadata such a C++ symbol, or a C++ source
filename. The `demangle` option may be used to demangle C++ symbols using LLVM.

The result is a series of graphs, each one designating a particular timer event,
specific to a source or a symbol whenever it is possible (ie. whenever metadata
is present in the `/args/detail` value of a timer event). Each graph compares
the evolution of these timer events in function of the benchmark size.

The graphs following were generated from the
[**ctbench** example](https://github.com/JPenuchot/ctbench/tree/joss/example)
on a Lenovo T470 with an Intel i5 6300U and 8GB of RAM. The compiler is Clang
14.0.6, and [Pyperf](https://pyperf.readthedocs.io/en/latest/system.html) was
used to turn off CPU frequency scaling.

![ExecuteCompiler](docs/images/ExecuteCompiler.svg){width=100%}

The first timer we want to look at is ExecuteCompiler, which is the total
compilation time. Starting from there we can go down in the timer event
hierarchy to take a look at frontend and backend execution times.

![Total Frontend](docs/images/Total_Frontend.svg){width=100%}

![Total Backend](docs/images/Total_Backend.svg){width=100%}

The backend is not being impacted here, supposedly because this is purely a
compile-time program, and the output program is empty. However this might not be
the case for all metaprograms, and metaprograms might have different impacts on
the backend as they may generate programs in different ways (ie. generate more
symbols, larger symbols, more data structures, etc.).

![Total InstantiateFunction](docs/images/Total_InstantiateFunction.svg){width=100%}

The Total Instantiate function timer is an interesting one as it explicitly
targets function instanciation time. Note that timers that are prefixed with
"Total" measure the total time spent in a timer section, regardless of the
specific symbol or source associated to its individual timer events.

![InstantiateFunction foovoid](docs/images/InstantiateFunction/foovoid.svg){width=100%}

Finally, we can take a look at `InstantiateFunction/foovoid.svg` which measures
the InstantiateFunction event time specifically for `foo<void>()`, which is our
driver template function. Using Perfetto UI to look at the timer event
hierarchy, we can validate that the timer event for this specific symbol
includes the InstantiateFunction time for all the symbols that may be
instantiated within this function.

This level of detail and granularity in the analysis of compile-time benchmarks
was never reached before, and may help us set good practices to improve the
compile-time performance of metaprograms.

# `ctbench`

Set of tools for comprehensive benchmarks based on clang's time-trace feature.

`ctbench` allows you to declare and generate compile-time benchmark batches for
given ranges, run them, aggregate time-trace reports, and plot them.
This allows you to get more comprehensive data than just compiler runtime
or single-size flame graphs.

Also, `ctbench` was made with the intents of **integration** and **reuse**.

## How-to

Get the contents of the project as you wish:
- [Git Submodule](
  https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html),
- [CMake FetchContent](
  https://cmake.org/cmake/help/latest/module/FetchContent.html),

then `add_subdirectory(external/ctbench)` or whatever in your `CMakeLists.txt`.
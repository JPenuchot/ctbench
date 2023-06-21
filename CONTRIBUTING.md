# Welcome to ctbench contributing guide

Thank you for investing your time in contributing to our project!

Read our [Code of Conduct](./CODE_OF_CONDUCT.md) to keep our community
approachable and respectable.

In this guide you will get an overview of the contribution workflow from opening
an issue, creating a PR, reviewing, and merging the PR.

## New contributor guide

To get an overview of the project, read the [README](README.md). Here are some
resources to help you get started with open source contributions:

- [Finding ways to contribute to open source on GitHub](
https://docs.github.com/en/get-started/exploring-projects-on-github/finding-ways-to-contribute-to-open-source-on-github) - [Set up
Git](https://docs.github.com/en/get-started/quickstart/set-up-git) - [GitHub
flow](https://docs.github.com/en/get-started/quickstart/github-flow) -
[Collaborating with pull
requests](https://docs.github.com/en/github/collaborating-with-pull-requests)

## Getting started

To navigate the codebase using tools based on ClangD,
`compile_commands.json` can be generated in the `build/` folder
by running `cmake --preset dev`.

### Issues

#### Create a new issue

*All issues are welcome.*

If you feel like you need support after reading the docs,
then the docs were not clear enough and *we need to hear about it*.
Do not censor yourself, and please raise the issue.

If you think you found a bug,
please make sure your issue contains the following:

- A short descrpition of the bug
- A short description of your system (OS, distro, etc.)
- Steps to reproduce
- Expected behavior
- Actual behavior

Feature requests are welcome.

#### Solve an issue

Scan through our [existing issues](https://github.com/github/docs/issues) to
find one that interests you. You can narrow down the search using `labels` as
filters. See [Labels](/contributing/how-to-use-labels.md) for more information.
As a general rule, we don’t assign issues to anyone. If you find an issue to
work on, you are welcome to open a PR with a fix.

### Make Changes

### Checklist for C++ code contributions

- Every C++ function must have a Doxygen explainer that helps
  understand its purpose without looking at the implementation.

- C++ code must be formatted using `clang-format` using the default settings
  (ie. LLVM style).
  `// clang-format off` and `// clang-format on` may be used in cases where
  it can improve readability.

- Names are stylized in `snake_case`.

- Avoid use of `auto` variable types to make types visible in the code.

- Variables names are long and clear enough.

- Code is largely commented.

- Core type aliases are defined in `grapher/include/grapher/core.cpp`
  for things like JSON objects, maps, and multimaps.
  Please make sure you use them.

- Functions for JSON value or reference access are declared in
  `grapher/include/grapher/utils/json.hpp`.
  Please make sure you use them too.

### Checklist for CMake API code contributions

- Every CMake function must have a Markdown explainer that helps
  understand its purpose without looking at the implementation.

- Public API functions must begin with `ctbench_`.

- Internal API functions must begin with `_ctbench_`.

#### CMake API documentation explainer

A preprocessor is used to generate a Markdown file (see `cmake-doc-extractor/`).
Lines beginning with `#!` will be part of the public API documentation, whereas
lines beginning with `#@` will be part of the internal API documentation.

- Public CMake API documentation looks like the following:

```cmake
#!
#! ### some_public_api_function(name foo bar)
#!
#! A paragraph describing my superb function.
#!
#! - `foo`: Description of parameter foo
#! - `bar`: Description of parameter bar
#!
```

- Internal CMake API documentation looks like the following:

```cmake
#@
#@ ### _some_internal_api_function(name foo bar)
#@
#@ A paragraph describing my superb function.
#@
#@ - `foo`: Description of parameter foo
#@ - `bar`: Description of parameter bar
#@
```

#### Adding a new plotter

For plotter implementation please look at
`grapher/include/grapher/plotters/debug.hpp` as a reference.
Every plotter must satisfy the virtual interface defined in
`grapher/include/grapher/plotters/plotter_base.hpp`.

Take a look at `grapher/include/grapher/plotters/plotters.hpp`
and make sure your plotter appears in all following definitions:

- `plotter_config`: plotter documentation page
- `plotter_type_t`: plotter type enumeration
- `plotter_name_map`: string to plotter type map
- `plotter_cl_values`: CLI parsing options
- `plotter_type_to_plotter`: plotter type to `plotter_t` value map function

### Pull Request

*Pull requests are always welcome.*

Feel free to open draft pull requests at any time to trigger the CI
and test your code on the supported platforms.
You may remove the draft status if you feel like your PR is ready
or needs to be reviewed or discussed.

Thanks in advance for your time, and thank you for reading these guidelines!

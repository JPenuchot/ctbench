#pragma once

/// \file
/// Data types for benchmark results representation.

#include <filesystem>
#include <string>
#include <tuple>

#include <nlohmann/json.hpp>

namespace grapher {

using json_t = nlohmann::json;

/// Set of results for a benchmark case iteration of a given size.
struct benchmark_iteration_t {
  /// Size of the benchmark instance
  unsigned size;

  /// Data for each repetition of the benchmark iteration
  std::vector<std::filesystem::path> samples;
};

/// Represents results for a benchmark case as a series of benchmark iterations.
struct benchmark_case_t {
  /// Name of the benchmark case
  std::string name;

  /// Instances of various sizes of the benchmark case
  std::vector<benchmark_iteration_t> iterations;
};

/// Represents a set of benchmark cases for comparison
using benchmark_set_t = std::vector<benchmark_case_t>;

} // namespace grapher

#pragma once

/// \file
/// Data types for benchmark results representation.

#include <string>
#include <tuple>

#include <nlohmann/json.hpp>

namespace grapher {

/// Set of results for a benchmark case instanciated at a given size.
struct benchmark_instance_t {
  /// Size of the benchmark instance
  unsigned size;

  /// Data series
  std::vector<nlohmann::json> iterations;
};

/// Represents results for a benchmark case as a series of benchmark instances.
struct benchmark_case_t {
  /// Name of the benchmark case
  std::string name;

  /// Instances of various sizes of the benchmark case
  //std::vector<benchmark_instance_t> instances;
  std::vector<benchmark_instance_t> instances;
};

/// Represents a set of benchmark cases for comparison
using benchmark_set_t = std::vector<benchmark_case_t>;

} // namespace grapher

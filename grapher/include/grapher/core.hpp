#pragma once

/// \file
/// Data types for benchmark results representation.

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include <boost/container/flat_map.hpp>

namespace grapher {

/// Alias type for JSON objects.
using json_t = nlohmann::basic_json<boost::container::flat_map>;

// `time cmake --build --preset bench` results using different containers
// (poacher/brainfuck project, pre-built benchmark targets):
// - boost::container::flat_map -> 78.05 secs
// - std::map -> 87.08 secs
// - boost::container::map -> 80.16 secs

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

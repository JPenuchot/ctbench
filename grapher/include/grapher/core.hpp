#pragma once

/// \file
/// Data types for benchmark results representation.

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>

#include <boost/container/flat_map.hpp>

namespace grapher {

// Basic container types

template <typename KeyType, typename ValueType,
          typename CompareType = std::less<KeyType>>
using map_t = boost::container::flat_map<KeyType, ValueType, CompareType>;

template <typename KeyType, typename ValueType,
          typename CompareType = std::less<KeyType>>
using multimap_t =
    boost::container::flat_multimap<KeyType, ValueType, CompareType>;

/// Alias type for JSON objects.
using json_t = nlohmann::basic_json<boost::container::flat_map>;

// `time cmake --build --preset bench` results using different containers
// (poacher/brainfuck project, pre-built benchmark targets):
// - boost::container::flat_map -> 78.05 secs
// - std::map -> 87.08 secs
// - boost::container::map -> 80.16 secs

/// Results for a benchmark case instantiated at a given size.
struct benchmark_instance_t {
  /// Size at which the benchmark is instantiated at
  unsigned size;

  /// Data for each repetition of the benchmark iteration
  std::vector<std::filesystem::path> repetitions;
};

/// Represents results for a benchmark case as a series of benchmark instances.
struct benchmark_case_t {
  /// Name of the benchmark case
  std::string name;

  /// Instances of various sizes of the benchmark case
  std::vector<benchmark_instance_t> instances;
};

/// Represents a set of benchmark cases for comparison
using benchmark_set_t = std::vector<benchmark_case_t>;

} // namespace grapher

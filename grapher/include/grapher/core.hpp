#pragma once

/// \file
/// Core data types for benchmark representation.

#include <string>
#include <tuple>

#include <nlohmann/json.hpp>

namespace grapher {

using measure_t = unsigned;

/// Represents a benchmark entry, which is caracterized by a size and the
/// benchmark results as JSON content.
struct entry_t {
  unsigned size;
  std::vector<nlohmann::json> data;
};

/// Represents a benchmark, which is a series of entries.
struct benchmark_t {
  std::string name;
  std::vector<entry_t> entries;
};

/// Represents a category, which is just a series of benchmarks that must be
/// plotted at the same scale.
using category_t = std::vector<benchmark_t>;

} // namespace grapher

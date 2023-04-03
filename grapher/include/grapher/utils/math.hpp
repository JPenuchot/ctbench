#pragma once

#include <numeric>
#include <vector>

#include <grapher/core.hpp>
#include <grapher/utils/error.hpp>

/// Math functions for statistics on time-trace data.
namespace grapher::math {

/// Conputes average of a series of value_t values.
inline value_t average(std::vector<value_t> const &values) {
  grapher::check(!values.empty(), "Cannot compute average on empty series.");
  return std::reduce(values.begin(), values.end()) / values.size();
}

/// Conputes median of a series of value_t values.
inline value_t median(std::vector<value_t> values) {
  check(!values.empty(), "Cannot compute median of an empty vector.");
  std::ranges::sort(values);
  bool const y_values_even = (values.size() & std::size_t(1)) == 0;
  std::size_t const mid = values.size() / 2;

  return y_values_even ? ((values[mid - 1] + values[mid]) / 2) : (values[mid]);
}

// TODO:
/// Computes standard deviation of a series of value_t values.
// inline value_t compute_stddev(std::vector<value_t> const &values)

} // namespace grapher::math

#pragma once

#include <cmath>
#include <numeric>
#include <vector>

#include <grapher/core.hpp>
#include <grapher/utils/error.hpp>

/// Math functions for statistics on time-trace data.
namespace grapher::math {

/// Conputes average of a series of value_t values.
inline double average(std::vector<value_t> const &values) {
  grapher::check(!values.empty(), "Cannot compute average on empty series.");
  return double(std::reduce(values.begin(), values.end())) /
         double(values.size());
}

/// Conputes median of a series of value_t values.
inline double median(std::vector<value_t> values) {
  check(!values.empty(), "Cannot compute median of an empty vector.");
  std::ranges::sort(values);
  bool const y_values_even = (values.size() & std::size_t(1)) == 0;
  std::size_t const mid = values.size() / 2;

  return y_values_even ? ((double(values[mid - 1]) + double(values[mid])) / 2)
                       : double(values[mid]);
}

/// Computes standard deviation of a series of value_t values.
inline double stddev(std::vector<value_t> const &values) {
  grapher::check(!values.empty(), "Cannot compute stddev on empty series.");

  double const count = double(values.size());
  double const mean = double(std::reduce(values.begin(), values.end())) / count;

  double acc = 0;
  for (value_t const &element : values) {
    acc += (double(element) - mean) * (double(element) - mean);
  }

  return std::sqrt(acc / count);
}

} // namespace grapher::math

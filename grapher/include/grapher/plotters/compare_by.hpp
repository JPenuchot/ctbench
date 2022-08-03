#pragma once

#include "grapher/plotters/plotter_base.hpp"

namespace grapher::plotters {

/// Generates a benchmark comparison graph for every matching key built from the
/// list of JSON pointers.
///
/// JSON config parameters:
/// - `key_ptrs` (string array): pointers to JSON values to use as a key
/// - `value_ptr` (`string`): pointer to the JSON value to measure
/// - `draw_average` (`bool`): enable average curve drawing
/// - `draw_points` (`bool`): enable value point drawing
/// - `demangle` (`bool`): demangle C++ symbol names
///
/// \copydoc base_default_config
///
/// Example config:
/// \code{.json}
/// {
///   "draw_average": true,
///   "draw_points": true,
///   "height": 500,
///   "key_ptrs": [
///     "/name",
///     "/args/detail"
///   ],
///   "legend_title": "Timings",
///   "plot_file_extensions": [
///     ".svg",
///     ".png"
///   ],
///   "plotter": "compare_by",
///   "value_ptr": "/dur",
///   "width": 1500,
///   "x_label": "Benchmark size factor",
///   "y_label": "Time (µs)"
/// }
/// \endcode

struct plotter_compare_by_t : plotter_base_t {
  void plot(benchmark_set_t const &bset, std::filesystem::path const &dest,
            grapher::json_t const &config) const override;

  grapher::json_t get_default_config() const override;
};

} // namespace grapher::plotters

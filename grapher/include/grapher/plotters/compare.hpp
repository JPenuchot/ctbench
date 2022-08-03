#pragma once

#include "grapher/plotters/plotter_base.hpp"

namespace grapher::plotters {

/// For each group descriptor, generates a graph comparing all benchmark cases
/// in the set.
///
/// Plotter-specific JSON parameters:
/// - `value_json_pointer` (`string`): pointer to JSON value to measure
/// - `draw_average` (`bool`): enable average curve drawing
/// - `draw_points` (`bool`): enable point value drawing
/// - `group_descriptors` (group descriptors): see group_descriptor_t
/// documentation
///
/// \copydetails base_default_config
///
/// Example config:
/// \code{.json}
/// {
///   "draw_average": true,
///   "draw_points": true,
///   "group_descriptors": [
///     {
///       "name": "All",
///       "predicates": [
///         {
///           "pointer": "/name",
///           "regex": "*",
///           "type": "regex"
///         }
///       ]
///     }
///   ],
///   "height": 500,
///   "legend_title": "Timings",
///   "plot_file_extensions": [
///     ".svg",
///     ".png"
///   ],
///   "plotter": "compare",
///   "value_json_pointer": "/dur",
///   "width": 1500,
///   "x_label": "Benchmark size factor",
///   "y_label": "Time (µs)"
/// }
/// \endcode

struct plotter_compare_t : plotter_base_t {
  void plot(benchmark_set_t const &bset, std::filesystem::path const &dest,
            grapher::json_t const &config) const override;

  grapher::json_t get_default_config() const override;
};

} // namespace grapher::plotters

#pragma once

#include "grapher/plotters/plotter_base.hpp"

namespace grapher::plotters {

/// For each benchmark in the category, generates a stakcked curve graph
/// where each curve corresponds to a matcher in the matchers JSON field.
///
/// Plotter-specific JSON parameters:
/// - `value_json_pointer` (`string`): pointer to JSON value to measure
/// - `group_descriptors` (group descriptors): see group_descriptor_t
/// documentation
///
/// \copydetails base_default_config
///
/// Example config:
/// \code{.json}
/// {
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
///   "name_json_pointer": "/name",
///   "plot_file_extensions": [
///     ".svg",
///     ".png"
///   ],
///   "plotter": "stack",
///   "value_json_pointer": "/dur",
///   "width": 1500,
///   "x_label": "Benchmark size factor",
///   "y_label": "Time (µs)"
/// }
/// \endcode

struct plotter_stack_t : plotter_base_t {
  void plot(benchmark_set_t const &bset, std::filesystem::path const &dest,
            grapher::json_t const &config) const override;

  grapher::json_t get_default_config() const override;
};

} // namespace grapher::plotters

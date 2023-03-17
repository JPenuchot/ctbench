#pragma once

#include "grapher/plotters/plotter_base.hpp"

namespace grapher::plotters {

/// Debug plotter. Outputs various statistics on benchmark categories to debug
/// category building or traversal issues.
///
/// Example config:
/// \code{.json}
/// {
///   "plotter": "debug"
/// }
/// \endcode

struct plotter_debug_t : public plotter_base_t {
  void plot(const benchmark_set_t &input_benchmark_set,
            const std::filesystem::path &destination_file,
            const grapher::json_t &config) const override;

  grapher::json_t get_default_config() const override;
};

} // namespace grapher::plotters

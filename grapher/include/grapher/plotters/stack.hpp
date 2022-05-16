#pragma once

#include "grapher/plotters/plotter_base.hpp"

namespace grapher::plotters {

/// \ingroup plotters

/// Generates one plot per benchmark where all the targeted features are
/// visualized as stacked curves.
struct plotter_stack_t : plotter_base_t {
  void plot(benchmark_set_t const &bset, std::filesystem::path const &dest,
            grapher::json_t const &config) const override;

  grapher::json_t get_default_config() const override;
};

} // namespace grapher::plotters

#pragma once

#include "grapher/plotters/plotter_i.hpp"

namespace grapher::plotters {

/// \ingroup plotters

/// Generates one plot per benchmark where all the targeted features are
/// visualized as stacked curves.
struct plotter_stack_t : plotter_i {
  void plot(benchmark_set_t const &bset, std::filesystem::path const &dest,
            grapher::json_t const &config) const override;

  std::string_view get_help() const override;

  grapher::json_t get_default_config() const override;
};

} // namespace grapher::plotters

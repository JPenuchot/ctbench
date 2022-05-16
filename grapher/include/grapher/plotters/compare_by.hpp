#pragma once

#include "grapher/plotters/plotter_i.hpp"

namespace grapher::plotters {

/// \ingroup plotters

struct plotter_compare_by_t : plotter_i {
  void plot(benchmark_set_t const &bset, std::filesystem::path const &dest,
            grapher::json_t const &config) const override;

  grapher::json_t get_default_config() const override;
};

} // namespace grapher::plotters

#pragma once

#include "grapher/plotters/plotter_i.hpp"

namespace grapher::plotters {

/// \ingroup plotters
/// Debug plotter, outputs statistics on benchmark categories
struct plotter_debug_t : public plotter_i {
  void plot(benchmark_set_t const &bset, std::filesystem::path const &dest,
            nlohmann::json const &config) const override;

  std::string_view get_help() const override;

  nlohmann::json get_default_config() const override;
};

} // namespace grapher::plotters

#pragma once

#include <grapher/plotters/plotter_i.hpp>

namespace grapher {

struct stacked_graph_t : plotter_i {
  void plot(category_t const &cat, std::filesystem::path const &dest,
            nlohmann::json const &config) override;

  std::string_view help() const override;
};

} // namespace grapher

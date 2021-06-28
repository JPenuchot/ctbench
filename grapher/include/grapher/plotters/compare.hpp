#pragma once

#include <grapher/plotters/plotter_i.hpp>

namespace grapher {

struct comparative_graph_t : plotter_i {
  void operator()(category_t const &cat, std::filesystem::path const &dest,
                  std::vector<nlohmann::json> const &matcher_set,
                  nlohmann::json::json_pointer feature_value_jptr,
                  nlohmann::json::json_pointer feature_name_jptr,
                  graph_config_t const &config) override;
};

} // namespace grapher

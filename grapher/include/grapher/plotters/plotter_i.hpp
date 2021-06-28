#pragma once

#include <filesystem>

#include <grapher/core.hpp>

namespace grapher {

/// Data structure for plotting parameters (will be added in the future)
using graph_config_t = nlohmann::json;

struct plotter_i {
  virtual void operator()(category_t const &cat,
                          std::filesystem::path const &dest,
                          std::vector<nlohmann::json> const &matcher_set,
                          nlohmann::json::json_pointer feature_value_jptr,
                          nlohmann::json::json_pointer feature_name_jptr,
                          graph_config_t const &config) = 0;

  virtual ~plotter_i() = default;
};

} // namespace grapher

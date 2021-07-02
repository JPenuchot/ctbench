#pragma once

#include <filesystem>
#include <memory>
#include <string_view>

#include <grapher/core.hpp>

namespace grapher {

/// Common interface for plotters.
struct plotter_i {
  virtual void plot(category_t const &cat, std::filesystem::path const &dest,
                    nlohmann::json const &config) const = 0;
  virtual std::string_view help() const = 0;
  virtual nlohmann::json get_default_config() const = 0;

  virtual ~plotter_i() = default;
};

using plotter_t = std::unique_ptr<plotter_i>;

} // namespace grapher

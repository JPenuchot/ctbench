#pragma once

#include <filesystem>
#include <string_view>

#include <grapher/core.hpp>

namespace grapher {

/// Common interface for plotters.
struct plotter_i {
  virtual void plot(category_t const &cat, std::filesystem::path const &dest,
                    nlohmann::json const &config) = 0;

  virtual std::string_view help() const { return "To be implemented."; };

  virtual ~plotter_i() = default;
};

} // namespace grapher

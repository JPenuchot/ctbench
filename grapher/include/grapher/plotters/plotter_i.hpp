#pragma once

#include <filesystem>
#include <memory>
#include <string_view>

#include "grapher/core.hpp"

namespace grapher {

/// Common interface for plotters.
struct plotter_i {
  virtual ~plotter_i() = default;

  /// Plots a given category at the given destination.
  /// A JSON file is passed as a config.
  virtual void plot(category_t const &cat, std::filesystem::path const &dest,
                    nlohmann::json const &config) const = 0;

  /// Prints plotter-specific help.
  virtual std::string_view get_help() const = 0;

  /// Returns plotter-specific default config.
  virtual nlohmann::json get_default_config() const = 0;
};

/// Polymorphic representation of a plotter.
using plotter_t = std::unique_ptr<plotter_i>;

} // namespace grapher

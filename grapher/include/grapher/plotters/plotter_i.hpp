#pragma once

#include <filesystem>
#include <memory>
#include <string_view>

#include "grapher/core.hpp"

namespace grapher {

/// \defgroup plotters Plotters
/// Plotters are objects that inherit the plotter_i virtual interface, and thus
/// override the plotter_i::plot, plotter_i::get_help, and
/// plotter_i::get_default_config methods.
///
/// They're used to generate plots from a grapher::category_t object, and are
/// configured by a nlohmann::json object.

/// \ingroup plotters
/// Common interface for plotters.
struct plotter_i {
  virtual ~plotter_i() = default;

  /// Plots a given category at the given destination. It is parameterized by a
  /// JSON config file.
  virtual void plot(category_t const &cat, std::filesystem::path const &dest,
                    nlohmann::json const &config) const = 0;

  /// Prints plotter-specific user help message. This is also the best place to
  /// document plotter parameters.
  virtual std::string_view get_help() const = 0;

  /// Returns plotter-specific default config. Make sure you cover all the
  /// available parameters when overriding it.
  virtual nlohmann::json get_default_config() const = 0;
};

/// Polymorphic representation of a plotter.
using plotter_t = std::unique_ptr<plotter_i>;

} // namespace grapher

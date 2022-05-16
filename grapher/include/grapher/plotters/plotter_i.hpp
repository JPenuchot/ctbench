#pragma once

#include <filesystem>
#include <memory>
#include <string_view>

#include "grapher/core.hpp"

namespace grapher {

/// \defgroup plotters Plot generators

/// \addtogroup plotters
///
/// Plotters are objects that inherit the plotter_i virtual interface, and thus
/// override the plotter_i::plot, plotter_i::get_help, and
/// plotter_i::get_default_config methods.
///
/// They're used to generate plots from a grapher::category_t object and a
/// grapher::json_t object for configuration.
///
/// The plotter interface can also be used to implement other exportation modes
/// such as CSV, plain text, debug, or even HTML export if you want.

/// \ingroup plotters

/// Interface for plotters. Plotters should be able to:
/// - Plot a ctbench::category_t with a grapher::json_t configuration object,
/// - Output help as a std::string,
/// - Output a default config as a grapher::json_t object.
struct plotter_i {
  virtual ~plotter_i() = default;

  /// Plots a given ctbench::category_t at the given destination.
  /// It receives a grapher::json_t object as a config.
  virtual void plot(benchmark_set_t const &bset,
                    std::filesystem::path const &dest,
                    grapher::json_t const &config) const = 0;

  /// Returns a help message for end-users.
  virtual std::string get_help() const = 0;

  /// Returns a default config for end-users.
  virtual grapher::json_t get_default_config() const = 0;
};

/// Polymorphic representation of a plotter.
using plotter_t = std::unique_ptr<plotter_i>;

} // namespace grapher

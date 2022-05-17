#pragma once

#include <filesystem>
#include <memory>
#include <string_view>

#include "grapher/core.hpp"

namespace grapher {

/// Plotters are objects that inherit the plotter_base_t virtual interface, and
/// thus override the plotter_base_t::plot, and
/// plotter_base_t::get_default_config methods.
///
/// They're used to generate plots from a grapher::category_t object and a
/// grapher::json_t object for configuration.
///
/// The plotter interface can also be used to implement other exportation modes
/// such as CSV, plain text, debug, or even HTML export if you want.
///
/// Interface for plotters. Plotters should be able to:
/// - Plot a ctbench::category_t with a grapher::json_t configuration object,
/// - Output a default config as a grapher::json_t object.
struct plotter_base_t {
  virtual ~plotter_base_t() = default;

  /// Plots a given ctbench::category_t at the given destination.
  /// It receives a grapher::json_t object as a config.
  virtual void plot(benchmark_set_t const &bset,
                    std::filesystem::path const &dest,
                    grapher::json_t const &config) const = 0;

  /// Returns a default config for end-users.
  virtual grapher::json_t get_default_config() const = 0;
};

/// Polymorphic representation of a plotter.
using plotter_t = std::unique_ptr<plotter_base_t>;

} // namespace grapher

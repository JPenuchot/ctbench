#pragma once

#include <nlohmann/json.hpp>

#include <grapher/core.hpp>
#include <grapher/plotters/plotter_i.hpp>

/// Namespace for CLI-related functions. Call parse_cli_options before using any
/// other function from this namespace.
namespace grapher::main_cli {

/// Call this once to parse options and make other functions in grapher::cli
/// available.
bool parse_cli_options(int argc, char const *argv[]);

/// Builds a category from parsed arguments.
/// Requires parse_cli_options to be called before use.
grapher::category_t build_category();

/// Gets the config from parsed arguments.
/// Requires parse_cli_options to be called before use.
nlohmann::json get_config();

/// Returns the appropriate plotter from parsed arguments.
/// Requires parse_cli_options to be called before use.
std::unique_ptr<grapher::plotter_i> select_plotter();

} // namespace grapher::cli

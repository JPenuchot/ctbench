#pragma once

#include <grapher/plotters/plotter_i.hpp>

namespace cli {

/// Command line option
enum command_enum_t {
  generate_config_v,
  help_v,
};

bool parse_cli_options(int argc, char const *argv[]);

grapher::plotter_t get_plotter();
command_enum_t get_command();

} // namespace cli

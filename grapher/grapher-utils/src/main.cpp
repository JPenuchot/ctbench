#include <iostream>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

#include "grapher/plotters/plotter_i.hpp"
#include "grapher/plotters/plotters.hpp"

namespace cli {

namespace lc = llvm::cl;

lc::opt<grapher::plotter_type_t> plotter_opt("p", lc::Required,
                                             lc::desc("Plotter:"),
                                             grapher::plotter_cl_values);

enum command_enum_t {
  generate_config_v,
  help_v,
};

lc::opt<command_enum_t> command_opt(
    "c", lc::Required, lc::desc("Command:"),
    lc::values(lc::OptionEnumValue{"get-help", command_enum_t::help_v,
                                   "Get plotter help."},
               lc::OptionEnumValue{"get-default-config",
                                   command_enum_t::generate_config_v,
                                   "Output plotter default config."}));

} // namespace cli

int main(int argc, char const *argv[]) {
  llvm::cl::extrahelp("\nRun special plotter commands.\n");
  llvm::cl::ParseCommandLineOptions(argc, argv);

  cli::command_enum_t cmd = cli::command_opt.getValue();
  grapher::plotter_t plotter =
      grapher::plotter_type_to_plotter(cli::plotter_opt.getValue());

  switch (cmd) {
  case cli::generate_config_v:
    std::cout << plotter->get_default_config().dump(2);
    break;
  case cli::help_v:
    std::cout << plotter->get_help();
    break;
  }

  return 0;
}

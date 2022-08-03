#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

#include "grapher/plotters/plotters.hpp"

namespace cli {

namespace lc = llvm::cl;

lc::opt<grapher::plotter_type_t> plotter_opt("plotter", lc::Required,
                                             lc::desc("Plotter:"),
                                             grapher::plotter_cl_values);

enum command_enum_t {
  generate_config_v,
};

lc::opt<command_enum_t>
    command_opt("command", lc::Required, lc::desc("Command:"),
                lc::values(lc::OptionEnumValue{
                    "get-default-config", command_enum_t::generate_config_v,
                    "Output plotter default config."}));

} // namespace cli

int main(int argc, char const *argv[]) {
  llvm::cl::extrahelp("\nRun special plotter commands.\n");
  llvm::cl::ParseCommandLineOptions(argc, argv);

  grapher::plotter_t plotter =
      grapher::plotter_type_to_plotter(cli::plotter_opt.getValue());

  switch (cli::command_opt.getValue()) {
  case cli::generate_config_v:
    llvm::outs() << plotter->get_default_config().dump(2) << '\n';
    break;
  }

  return 0;
}

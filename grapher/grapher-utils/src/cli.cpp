#include <tuple>

#include <cli.hpp>

#include <grapher/plotters/plotter_i.hpp>
#include <grapher/plotters/plotters.hpp>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/CommandLine.h>

namespace cli {

namespace lc = llvm::cl;

lc::opt<llvm::StringRef> plotter_opt(lc::Positional, lc::Required,
                                     lc::desc("<Plotter>"));

lc::opt<command_enum_t> command_opt(
    lc::Positional, lc::Required, lc::desc("<Command>"),
    lc::values(lc::OptionEnumValue{"generate-config", generate_config_v,
                                   "Outputs default plotter config."},
               lc::OptionEnumValue{"help", help_v, "Outputs plotter help."}));

bool parse_cli_options(int argc, char const *argv[]) {
  return lc::ParseCommandLineOptions(argc, argv);
}

grapher::plotter_t get_plotter() {
  // TODO: Implement
  return std::make_unique<grapher::stacked_graph_t>();
}

command_enum_t get_command() { return command_opt.getValue(); }

} // namespace cli

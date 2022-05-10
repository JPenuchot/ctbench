#include <fstream>
#include <string>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

#include "grapher/core.hpp"
#include "grapher/plotters/plotters.hpp"
#include "grapher/utils/cli.hpp"
#include "grapher/utils/json.hpp"

namespace cli {
namespace lc = llvm::cl;

lc::opt<std::string> config_opt("config", lc::Required,
                                lc::desc("<config file>"));

lc::opt<std::string> output_folder_opt("output", lc::Required,
                                       lc::desc("<output folder>"));

lc::list<std::string> benchmark_path_list(lc::Positional, lc::OneOrMore,
                                          lc::desc("<input folders>"));
} // namespace cli

int main(int argc, char const *argv[]) {
  llvm::cl::ParseCommandLineOptions(argc, argv);

  // Get configed
  grapher::json_t config;
  {
    std::ifstream config_file(cli::config_opt.getValue());
    if (!config_file) {
      llvm::errs() << "Can't open config file: " << cli::config_opt.getValue()
                   << '\n';
      std::exit(1);
    }

    if (!(config_file >> config)) {
      llvm::errs() << "Invalid JSON config file: " << cli::config_opt.getValue()
                   << '\n';
      std::exit(1);
    }
  }

  // Acquire plotter
  grapher::plotter_t plotter =
      grapher::plotter_type_to_plotter(grapher::string_to_plotter_type(
          grapher::json_at_ref<grapher::json_t::string_t const &>(config,
                                                                  "plotter")));

  // Build cats
  grapher::benchmark_set_t bset =
      grapher::build_category(cli::benchmark_path_list);

  // Set destiny
  std::string dest = cli::output_folder_opt.getValue();

  // ???

  // Plotten
  plotter->plot(bset, dest, config);

  return 0;
}

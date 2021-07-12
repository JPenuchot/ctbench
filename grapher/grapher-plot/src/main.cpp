#include "grapher/cli-utils.hpp"
#include <fstream>
#include <llvm/Support/CommandLine.h>
#include <string_view>

#include <nlohmann/json.hpp>

#include <grapher/core.hpp>
#include <grapher/plotters/plotters.hpp>

namespace cli {
namespace lc = llvm::cl;

lc::opt<grapher::plotter_type_t> plotter_opt("p", lc::Required,
                                             lc::desc("Plotter:"),
                                             grapher::plotter_cl_values);

lc::opt<std::string> output_folder_opt("o", lc::Required,
                                       lc::desc("<output folder>"));

lc::list<std::string> benchmark_path_list(lc::Positional, lc::OneOrMore,
                                          lc::desc("<input folders>"));

lc::opt<std::string> config_opt("c", "config",
                                lc::desc("JSON configuration file."));
} // namespace cli

int main(int argc, char const *argv[]) {
  llvm::cl::ParseCommandLineOptions(argc, argv);

  // Acquire plotter
  grapher::plotter_t plotter =
      grapher::plotter_type_to_plotter(cli::plotter_opt.getValue());

  // Build cats
  grapher::category_t cat = grapher::build_category(cli::benchmark_path_list);

  // Get configed
  nlohmann::json config;
  {
    std::ifstream config_file(cli::config_opt.getValue());
    config_file >> config;
  }

  // Set destiny
  std::string dest = cli::output_folder_opt.getValue();

  // ???

  // Plotten
  plotter->plot(cat, dest, config);

  return 0;
}

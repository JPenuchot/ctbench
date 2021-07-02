#include <llvm/Support/CommandLine.h>
#include <string_view>

#include <nlohmann/json.hpp>

#include <grapher/core.hpp>
#include <grapher/main_cli.hpp>
#include <grapher/plotters/plotters.hpp>

int main(int argc, char const *argv[]) {
  grapher::main_cli::parse_cli_options(argc, argv);

  grapher::category_t cat = grapher::main_cli::build_category();
  nlohmann::json config = grapher::main_cli::get_config();
  std::string_view dest = grapher::main_cli::get_destination();
  grapher::plotter_t plotter = grapher::main_cli::select_plotter();

  plotter->plot(cat, dest, config);

  return 0;
}

#include <string_view>

#include <nlohmann/json.hpp>

#include <grapher/core.hpp>
#include <grapher/plotters/plotters.hpp>

#include <cli.hpp>

int main(int argc, char const *argv[]) {
  cli::parse_cli_options(argc, argv);

  // Acquire plotter
  grapher::plotter_t plotter = cli::select_plotter();

  // Build cats
  grapher::category_t cat = cli::build_category();

  // Get configed
  nlohmann::json config = cli::get_config();

  // Set destiny
  std::string dest = cli::get_destination();

  // ???

  // Plotten
  plotter->plot(cat, dest, config);

  return 0;
}

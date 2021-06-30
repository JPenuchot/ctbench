#include <nlohmann/json.hpp>

#include <grapher/core.hpp>
#include <grapher/main_cli.hpp>

int main(int argc, char const *argv[]) {
  grapher::main_cli::parse_cli_options(argc, argv);
  grapher::category_t cat = grapher::main_cli::build_category();
  nlohmann::json config = grapher::main_cli::get_config();

  // Output
  // grapher::stacked_graph(category, argv[out_id]);

  return 0;
}

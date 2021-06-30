#include <nlohmann/json.hpp>

#include <grapher/cli.hpp>
#include <grapher/core.hpp>

int main(int argc, char const *argv[]) {
  grapher::cli::parse_cli_options(argc, argv);
  grapher::category_t cat = grapher::cli::build_category();
  nlohmann::json config = grapher::cli::get_config();

  // Output
  // grapher::stacked_graph(category, argv[out_id]);

  return 0;
}

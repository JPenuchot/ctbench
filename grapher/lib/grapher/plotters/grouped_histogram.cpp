#include <vector>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

#include "grapher/json-utils.hpp"
#include "grapher/plot-utils.hpp"
#include "grapher/plotters/grouped_histogram.hpp"

namespace grapher::plotters {

std::string_view plotter_grouped_histogram_t::get_help() const {
  // TODO: Add doc
  return "TODO";
}

nlohmann::json plotter_grouped_histogram_t::get_default_config() const {
  nlohmann::json res = grapher::base_default_config();

  // Basic values, probably no need to change them
  res["value_json_pointer"] = "/dur";
  res["name_json_pointer"] = "/name";
  res["plot_file_extension"] = ".svg";

  // Some matchers as an example...
  res["matchers"].push_back({{"name", "Total ExecuteCompiler"}});
  res["matchers"].push_back({{"name", "Total Frontend"}});
  res["matchers"].push_back({{"name", "Total Source"}});
  res["matchers"].push_back({{"name", "Total ParseClass"}});
  res["matchers"].push_back({{"name", "Total InstantiateClass"}});
  res["matchers"].push_back({{"name", "Total Backend"}});
  res["matchers"].push_back({{"name", "Total ParseTemplate"}});
  res["matchers"].push_back({{"name", "Total OptModule"}});
  res["matchers"].push_back({{"name", "Total CodeGenPasses"}});
  res["matchers"].push_back({{"name", "Total PerModulePasses"}});
  res["matchers"].push_back({{"name", "Total PerFunctionPasses"}});
  res["matchers"].push_back({{"name", "Total PerformPendingInstantiations"}});

  return res;
}

void plotter_grouped_histogram_t::plot(const category_t &cat,
                                       const std::filesystem::path &dest,
                                       const nlohmann::json &config) const {
  nlohmann::json const default_config = this->get_default_config();
  std::vector<nlohmann::json> matcher_set;

  if (config.contains("matchers") && config["matchers"].is_array()) {
    matcher_set = std::vector<nlohmann::json>(config["matchers"]);
  } else {
    llvm::errs() << "Warning: No matcher was specified in the configuration "
                    "file. Falling back to default matchers.\n";
    matcher_set = std::vector<nlohmann::json>(default_config["matchers"]);
  }

  nlohmann::json::json_pointer feature_value_jptr(
      config.value("value_json_pointer", "/dur"));

  nlohmann::json::json_pointer feature_name_jptr(
      config.value("name_json_pointer", "/name"));
}

} // namespace grapher::plotters

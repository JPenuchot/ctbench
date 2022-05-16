#include <filesystem>

#include <nlohmann/json.hpp>
#include <sciplot/sciplot.hpp>

#include "grapher/core.hpp"
#include "grapher/plotters/plotter_i.hpp"
#include "grapher/utils/plot.hpp"

namespace grapher {

/// Default config file for plots
grapher::json_t const default_config = {
    {"width", 1500},
    {"height", 500},
    {"legend_title", "Timings"},
    {"x_label", "Benchmark size factor"},
    {"y_label", "Time (µs)"},
    {"plot_file_extensions", grapher::json_t::array({".svg", ".png"})},
};

std::string base_default_config_help() {
  return "General plot parameters:\n"
         "- width (int): output width\n"
         "- height (int): output height\n"
         "- legend_title (string): legend title\n"
         "- x_label (string): x axis label\n"
         "- y_label (string): y axis label\n"
         "- plot_file_extensions (array of string): list of extensions for "
         "the export\n";
}

sciplot::Plot &apply_config(sciplot::Plot &plot, grapher::json_t config) {
  // Dimensions
  if (config.contains("width") && config.contains("height")) {
    plot.size(config["width"], config["height"]);
  }

  // Labels
  if (config.contains("legend_title")) {
    plot.legend().atOutsideRightTop().title(config["legend_title"]);
  }

  if (config.contains("x_label")) {
    plot.xlabel(config["x_label"]);
  }

  if (config.contains("ylabel")) {
    plot.ylabel(config["y_label"]);
  }

  return plot;
}

grapher::json_t base_default_config() { return default_config; }

void save_plot(sciplot::Plot const &plot, std::string const &dest,
               grapher::json_t const &config) {
  namespace fs = std::filesystem;
  std::vector<std::string> plot_file_extensions = config.value(
      "plot_file_extensions", grapher::json_t::array({".svg", ".png"}));

  // Saving file for all extensions
  for (std::string const &extension : plot_file_extensions) {
    fs::path file_dest = dest + extension;
    fs::create_directories(file_dest.parent_path());

    constexpr std::size_t max_filename_size = 256;

    // Avoid filename hitting OS filename size limit (yes, this is bad)
    if (file_dest.filename().string().size() > max_filename_size) {
      std::string new_filename = file_dest.stem();
      new_filename.resize(max_filename_size - extension.size());
      file_dest.replace_filename(new_filename);
    }

    plot.save(dest + extension);
  }
}

} // namespace grapher

#include <algorithm>
#include <execution>
#include <fstream>

#include <nlohmann/json.hpp>
#include <sciplot/Canvas.hpp>
#include <sciplot/Figure.hpp>

#include <grapher/core.hpp>
#include <grapher/predicates.hpp>
#include <grapher/utils/json.hpp>
#include <grapher/utils/tracy.hpp>

namespace grapher {

/// Default group descriptor values:
///
/// \code{.js}
/// {
///   "group_descriptors": [
///     {
///       "name": "All",
///       "predicates": [
///         {
///           "pointer": "/name",
///           "regex": "*",
///           "type": "regex"
///         }
///       ]
///     }
///   ]
/// }
/// \endcode

group_descriptor_t get_default_group_descriptor() {
  return {.name = "All",
          .predicates = grapher::json_t::array({grapher::json_t{
              {"type", "regex"},
              {"pointer", "/name"},
              {"regex", "*"},
          }})};
}

std::vector<predicate_t> get_predicates(group_descriptor_t const &descriptor) {
  std::vector<predicate_t> predicates;

  predicates.reserve(descriptor.predicates.size());
  std::ranges::transform(descriptor.predicates, std::back_inserter(predicates),
                         get_predicate);
  return predicates;
}

grapher::json_t::array_t extract_group(group_descriptor_t const &descriptor,
                                       grapher::json_t::array_t const &events) {
  std::vector<predicate_t> predicates = get_predicates(descriptor);

  grapher::json_t::array_t res;

  std::ranges::copy_if(
      events, std::back_inserter(res), [&](grapher::json_t const &event) {
        return std::ranges::all_of(
            predicates, [&](predicate_t const &p) { return p(event); });
      });

  return res;
}

group_descriptor_t read_descriptor(grapher::json_t const &j) {
  return {.name = get_as_ref<grapher::json_t::string_t const &>(j, "name"),
          .predicates =
              get_as_ref<grapher::json_t::array_t const &>(j, "predicates")};
}

grapher::json_t group_descriptor_json(group_descriptor_t const &descriptor) {
  return {
      {"name", descriptor.name},
      {"predicates", descriptor.predicates},
  };
}

grapher::json_t::array_t
write_descriptors(std::vector<group_descriptor_t> const &descriptors) {
  grapher::json_t::array_t res;
  res.reserve(descriptors.size());

  std::transform(descriptors.begin(), descriptors.end(),
                 std::back_inserter(res), &group_descriptor_json);

  return res;
}

std::vector<group_descriptor_t>
read_descriptors(grapher::json_t::array_t const &list) {
  std::vector<group_descriptor_t> res;
  res.reserve(list.size());
  std::transform(list.begin(), list.end(), std::back_inserter(res),
                 &read_descriptor);
  return res;
}

std::vector<double> get_values(benchmark_instance_t const &instance,
                               std::vector<predicate_t> const &predicates,
                               grapher::json_t::json_pointer value_jptr) {
  std::vector<double> res(instance.repetitions.size());

  auto get_val = [&](std::filesystem::path const &repetition_path) -> double {
    // Extract events
    grapher::json_t j;
    {
      std::ifstream repetition_ifstream(repetition_path);
      repetition_ifstream >> j;
    }

    grapher::json_t::array_t const &events =
        get_as_ref<grapher::json_t::array_t const &>(j, "traceEvents");

    // Accumulate
    double val = 0.;
    for (grapher::json_t const &event : events) {
      if (std::all_of(predicates.begin(), predicates.end(),
                      [&](predicate_t const &p) -> bool { return p(event); })) {
        val += get_as_ref<json_t::number_unsigned_t const &>(event, value_jptr);
      }
    }
    return val;
  };

  std::transform(std::execution::par_unseq, instance.repetitions.begin(),
                 instance.repetitions.end(), res.begin(), get_val);

  return res;
}

void save_plot(sciplot::Plot2D plot, std::string const &dest,
               grapher::json_t const &config) {
  ZoneScoped;
  namespace fs = std::filesystem;

  apply_config(plot, config);

  std::vector<std::string> plot_file_extensions = config.value(
      "plot_file_extensions", grapher::json_t::array({".svg", ".png"}));

  sciplot::Figure figure{{plot}};
  sciplot::Canvas canvas{{figure}};

  // canvas.autoclean(false);
  // canvas.saveplotdata();

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

    if (config.contains("width") && config.contains("height")) {
      canvas.size(config["width"], config["height"]);
    }
    { // Scope to profile canvas saving specifically
      ZoneScoped;
      canvas.save(dest + extension);
    }
  }
}

sciplot::Plot &apply_config(sciplot::Plot &plot,
                            grapher::json_t const &config) {
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

  if (config.contains("y_label")) {
    plot.ylabel(config["y_label"]);
  }

  return plot;
}

grapher::json_t const default_config = {
    {"width", 1500},
    {"height", 500},
    {"legend_title", "Timings"},
    {"x_label", "Benchmark size factor"},
    {"y_label", "Time (µs)"},
    {"plot_file_extensions", grapher::json_t::array({".svg", ".png"})},
};

/// Common plot JSON parameters:
/// - `width` (`int`): Graph width
/// - `height` (`int`): Graph height
/// - `legend_title` (`string`): Graph legend title
/// - `x_label` (`string`): X axis label
/// - `y_label` (`string`): Y axis label
/// - `plot_file_extensions` (string array): List of extensions for the export
grapher::json_t base_default_config() { return default_config; }

} // namespace grapher

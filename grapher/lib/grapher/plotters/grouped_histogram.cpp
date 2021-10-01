#include <algorithm>
#include <iterator>
#include <llvm/Support/raw_ostream.h>
#include <nlohmann/json_fwd.hpp>
#include <regex>
#include <vector>

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <nlohmann/json.hpp>

#include <sciplot/sciplot.hpp>

#include "grapher/plotters/grouped_histogram.hpp"
#include "grapher/plotters/plotters.hpp"
#include "grapher/utils/json.hpp"
#include "grapher/utils/plot.hpp"

namespace grapher::plotters {

//==============================================================================
// constraint_t

/// Represents a constriant on a JSON object
struct constraint_t {
  nlohmann::json::json_pointer pointer;
  llvm::SmallString<16> regex;
};

nlohmann::json constraint_to_json(constraint_t const &c) {
  return {
      {"pointer", c.pointer},
      {"regex", c.regex},
  };
}

constraint_t json_to_constraint(nlohmann::json const &j) {
  return constraint_t{
      .pointer{j.value("pointer", "/")},
      .regex{j.value("regex", "*")},
  };
}

//==============================================================================
// histo_group_t

/// Representing a group in a grouped_histogram config.
struct histo_group_t {
  llvm::SmallString<16> name;
  llvm::SmallVector<constraint_t, 2> constraints;
};

nlohmann::json histo_group_to_json(histo_group_t const &g) {
  llvm::SmallVector<nlohmann::json, 2> constraints;
  constraints.reserve(g.constraints.size());

  std::transform(g.constraints.begin(), g.constraints.end(),
                 std::back_inserter(constraints), &constraint_to_json);

  return {
      {"name", g.name},
      {"constriants", constraints},
  };
}

histo_group_t json_to_histo_group(nlohmann::json const &j) {
  llvm::SmallVector<constraint_t, 2> constraints;
  llvm::SmallVector<nlohmann::json, 2> constraints_json =
      j.value("constraints", llvm::SmallVector<nlohmann::json, 2>{});
  constraints.reserve(constraints_json.size());
  std::transform(constraints_json.begin(), constraints_json.end(),
                 std::back_inserter(constraints), &json_to_constraint);

  return histo_group_t{
      .name{j.value("name", "")},
      .constraints{constraints},
  };
}

//==============================================================================
// grouped_histogram_config_t

/// Config object for a grouped_histgram_t
struct grouped_histogram_config_t {
  llvm::SmallVector<nlohmann::json, 8> matchers;
  llvm::SmallVector<histo_group_t, 8> groups;

  llvm::SmallString<8> feature_value_jptr;
  llvm::SmallString<8> feature_name_jptr;
};

nlohmann::json
grouped_histogram_config_to_json(grouped_histogram_config_t const &c) {
  llvm::SmallVector<nlohmann::json, 8> groups_json;

  groups_json.reserve(c.groups.size());
  std::transform(c.groups.begin(), c.groups.end(),
                 std::back_inserter(groups_json), &histo_group_to_json);

  return {
      {"matchers", c.matchers},
      {"groups", std::move(groups_json)},
  };
}

grouped_histogram_config_t
json_to_grouped_config_histogram(nlohmann::json const &j) {
  llvm::SmallVector<nlohmann::json, 8> groups_json =
      j.value("groups", llvm::SmallVector<nlohmann::json, 8>{});

  llvm::SmallVector<histo_group_t, 8> groups;
  groups.reserve(groups_json.size());

  std::transform(groups_json.begin(), groups_json.end(),
                 std::back_inserter(groups), &json_to_histo_group);

  return grouped_histogram_config_t{
      .matchers = j.value("matchers", get_default_matchers()),
      .groups = groups,
  };
}

grouped_histogram_config_t get_grouped_histogram_default_config() {
  return grouped_histogram_config_t{
      .matchers = get_default_matchers(),
      .groups =
          {
              histo_group_t{.name{"Default"},
                            .constraints{constraint_t{
                                .pointer{"/name"},
                                .regex{"*"},
                            }}},
          },
  };
}

//==============================================================================
// plotter_grouped_histogram_t

std::string_view plotter_grouped_histogram_t::get_help() const {
  // TODO: Add doc
  return "TODO";
}

nlohmann::json plotter_grouped_histogram_t::get_default_config() const {
  return merge_into(
      grapher::base_default_config(),
      grouped_histogram_config_to_json(get_grouped_histogram_default_config()));
}

void plotter_grouped_histogram_t::plot(const benchmark_set_t &cat,
                                       const std::filesystem::path &dest,
                                       const nlohmann::json &config) const {
  grouped_histogram_config_t histo_config =
      json_to_grouped_config_histogram(config);

  for (histo_group_t const &g : histo_config.groups) {
    for (auto const &[name, entries] : cat) {
      for (auto const &[size, iterations] : entries) {
        for (auto const &iteration : iterations) {
          // ...
          if (!iteration.contains("traceEvents") ||
              !iteration["traceEvents"].is_array()) {
            llvm::errs() << "[WARNING] Invalid iteration data: no traceEvents "
                            "array field.\n";
            continue;
          }
        }
      }
    }
  }
}

} // namespace grapher::plotters

#pragma once

#include <grapher/predicates.hpp>

#include <vector>

#include <nlohmann/json.hpp>

/// \file
/// Common structure and function definitions that can be used for plotter
/// configuration.

namespace grapher {

/// Named set of constraint.
struct group_descriptor_t {
  std::string name;
  std::vector<nlohmann::json> predicates;
};

group_descriptor_t get_default_group_descriptor();

std::vector<predicate_t> get_predicates(group_descriptor_t const &descriptor);

std::vector<nlohmann::json>
extract_group(group_descriptor_t const &descriptor,
              std::vector<nlohmann::json> const &events);

group_descriptor_t json_to_group_descriptor(nlohmann::json const &j);

nlohmann::json group_descriptor_json(group_descriptor_t const &descriptor);

std::vector<nlohmann::json>
write_descriptors(std::vector<group_descriptor_t> const &descriptors);

std::vector<group_descriptor_t>
read_descriptors(std::vector<nlohmann::json> const &list);

} // namespace grapher

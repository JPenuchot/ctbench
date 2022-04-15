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
  std::vector<grapher::json_t> predicates;
};

group_descriptor_t get_default_group_descriptor();

std::vector<predicate_t> get_predicates(group_descriptor_t const &descriptor);

std::vector<grapher::json_t>
extract_group(group_descriptor_t const &descriptor,
              std::vector<grapher::json_t> const &events);

group_descriptor_t json_to_group_descriptor(grapher::json_t const &j);

grapher::json_t group_descriptor_json(group_descriptor_t const &descriptor);

std::vector<grapher::json_t>
write_descriptors(std::vector<group_descriptor_t> const &descriptors);

std::vector<group_descriptor_t>
read_descriptors(std::vector<grapher::json_t> const &list);

} // namespace grapher

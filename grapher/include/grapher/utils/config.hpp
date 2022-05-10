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
  grapher::json_t::array_t predicates;
};

/// Returns a default group descriptor as an example.
group_descriptor_t get_default_group_descriptor();

/// Extracts predicates from a group descriptor.
std::vector<predicate_t> get_predicates(group_descriptor_t const &descriptor);

grapher::json_t::array_t extract_group(group_descriptor_t const &descriptor,
                                       grapher::json_t::array_t const &events);

/// Exports a single group descriptor into a JSON object.
grapher::json_t group_descriptor_json(group_descriptor_t const &descriptor);

/// Exports a vector of group descriptors into a JSON object.
grapher::json_t::array_t
write_descriptors(std::vector<group_descriptor_t> const &descriptors);

/// Reads a single descriptor.
group_descriptor_t read_descriptor(grapher::json_t const &j);

/// Reads descriptors from a predicate list.
std::vector<group_descriptor_t>
read_descriptors(grapher::json_t::array_t const &list);

} // namespace grapher

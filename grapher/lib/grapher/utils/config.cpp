#include "grapher/utils/config.hpp"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <iterator>
#include <ranges>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>
#include <vector>

#include "grapher/predicates.hpp"
#include "grapher/utils/json.hpp"

namespace grapher {

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

std::vector<grapher::json_t>
extract_group(group_descriptor_t const &descriptor,
              std::vector<grapher::json_t> const &events) {
  std::vector<predicate_t> predicates = get_predicates(descriptor);

  std::vector<grapher::json_t> res;

  std::ranges::copy_if(
      events, std::back_inserter(res), [&](grapher::json_t const &event) {
        return std::ranges::all_of(
            predicates, [&](predicate_t const &p) { return p(event); });
      });

  return res;
}

group_descriptor_t json_to_group_descriptor(grapher::json_t const &j) {
  return {.name = json_value<std::string>(j, "name"),
          .predicates =
              json_value<std::vector<grapher::json_t>>(j, "predicates")};
}
grapher::json_t group_descriptor_json(group_descriptor_t const &descriptor) {
  return {
      {"name", descriptor.name},
      {"predicates", descriptor.predicates},
  };
}

std::vector<grapher::json_t>
write_descriptors(std::vector<group_descriptor_t> const &descriptors) {
  std::vector<grapher::json_t> res;
  res.reserve(descriptors.size());

  for (group_descriptor_t const &d : descriptors) {
    res.push_back(group_descriptor_json(d));
  }
  return res;
}

std::vector<group_descriptor_t>
read_descriptors(std::vector<grapher::json_t> const &list) {
  std::vector<group_descriptor_t> res;
  res.reserve(list.size());
  std::transform(list.begin(), list.end(), std::back_inserter(res),
                 &json_to_group_descriptor);
  return res;
}

} // namespace grapher

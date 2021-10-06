#include "grapher/utils/config.hpp"

#include <algorithm>
#include <iterator>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

#include "grapher/predicates.hpp"
#include "grapher/utils/json.hpp"

namespace grapher {

group_descriptor_t get_default_group_descriptor() {
  return {.name = "All",
          .constraints = nlohmann::json::array({
              {"type", "regex"},
              {"pointer", "/name"},
              {"regex", "*"},
          })};
}

std::vector<nlohmann::json>
extract_group(group_descriptor_t const &descriptor,
              std::vector<nlohmann::json> const &events) {
  std::vector<predicate_t> predicates;
  predicates.reserve(descriptor.constraints.size());

  std::transform(descriptor.constraints.begin(), descriptor.constraints.end(),
                 std::back_inserter(predicates), &get_predicate);

  auto check_predicates = [&](nlohmann::json const &event) -> bool {
    for (predicate_t const &p : predicates) {
      if (!p(event)) {
        return false;
      }
    }
    return true;
  };

  std::vector<nlohmann::json> res;
  std::copy_if(events.begin(), events.end(), std::back_inserter(res),
               check_predicates);

  return res;
}

group_descriptor_t json_to_group_descriptor(nlohmann::json const &j) {
  return {.name = json_value<std::string>(j, "name"),
          .constraints =
              json_value<std::vector<nlohmann::json>>(j, "constraints")};
}
nlohmann::json group_descriptor_json(group_descriptor_t const &descriptor) {
  return {
      {"name", descriptor.name},
      {"constrinats", descriptor.constraints},
  };
}

std::vector<nlohmann::json>
write_descriptors(std::vector<group_descriptor_t> const &descriptors) {
  std::vector<nlohmann::json> res;
  res.reserve(descriptors.size());

  for (group_descriptor_t const &d : descriptors) {
    res.push_back(group_descriptor_json(d));
  }
  return res;
}

std::vector<group_descriptor_t>
read_descriptors(std::vector<nlohmann::json> const &list) {
  std::vector<group_descriptor_t> res;
  res.reserve(list.size());
  std::transform(list.begin(), list.end(), std::back_inserter(res),
                 &json_to_group_descriptor);
  return res;
}

} // namespace grapher

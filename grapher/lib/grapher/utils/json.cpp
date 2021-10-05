#include "grapher/utils/json.hpp"

#include <iostream>
#include <optional>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>

#include "grapher/predicates.hpp"
#include "grapher/utils/config.hpp"

namespace grapher {

std::vector<double> get_values(entry_t const &entry,
                               group_descriptor_t const &descriptor,
                               nlohmann::json::json_pointer value_jptr) {
  std::vector<double> res;
  res.resize(entry.data.size());

  for (nlohmann::json const &iteration : entry.data) {
    double val = 0;
    std::vector<nlohmann::json> events =
        json_value<std::vector<nlohmann::json>>(iteration, "traceEvents");

    std::vector<nlohmann::json> matching_events =
        extract_group(descriptor, events);

    for (auto const &event : matching_events) {
      val += json_value<double>(event, value_jptr);
    }

    res.push_back(val);
  }

  return res;
}

nlohmann::json merge_into(nlohmann::json a, nlohmann::json const &b) {
  for (nlohmann::json const b_flat = b.flatten();
       auto const &[k_ptr, v] : b_flat.items()) {
    a[nlohmann::json::json_pointer(k_ptr)] = v;
  }
  return a;
}

} // namespace grapher

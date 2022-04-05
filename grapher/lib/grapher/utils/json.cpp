#include "grapher/utils/json.hpp"

#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <optional>

#include <llvm/Support/raw_ostream.h>

#include <nlohmann/json.hpp>
#include <pstl/glue_execution_defs.h>

#include "grapher/predicates.hpp"
#include "grapher/utils/config.hpp"

namespace grapher {

std::vector<double> get_values(benchmark_iteration_t const &iteration,
                               group_descriptor_t const &descriptor,
                               nlohmann::json::json_pointer value_jptr) {
  std::vector<double> res(iteration.samples.size());

  auto get_val = [&](std::filesystem::path const &repetition_path) -> double {
    // Extract events
    nlohmann::json j;
    {
      std::ifstream repetition_ifstream(repetition_path);
      repetition_ifstream >> j;
    }

    if (!j.contains("traceEvents") || !j["traceEvents"].is_array() ||
        j["traceEvents"].empty()) {
      return 0.;
    }

    nlohmann::json::array_t const &events =
        j["traceEvents"].get_ref<nlohmann::json::array_t const &>();

    std::vector<predicate_t> predicates = get_predicates(descriptor);

    // Accumulate
    double val = 0.;
    for (nlohmann::json const &event : events) {
      if (std::all_of(predicates.begin(), predicates.end(),
                      [&](predicate_t const &p) -> bool { return p(event); })) {
        val += json_value<double>(event, value_jptr);
      }
    }
    return val;
  };

  std::transform(std::execution::par_unseq, iteration.samples.begin(),
                 iteration.samples.end(), res.begin(), get_val);

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

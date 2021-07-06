#include <grapher/json-utils.hpp>

namespace grapher {

nlohmann::json::const_iterator
find_matching(nlohmann::json::const_iterator begin,
              nlohmann::json::const_iterator end,
              nlohmann::json const &matcher) {
  auto const flat_matcher = matcher.flatten();

  return std::find_if(begin, end, [&](nlohmann::json const &j) -> bool {
    auto const flat_j = j.flatten();

    for (auto const &i : flat_matcher.items()) {
      if (auto it = flat_j.find(i.key());
          it == flat_j.end() || *it != i.value()) {
        return false;
      }
    }
    return true;
  });
}

std::optional<double> get_average(std::vector<nlohmann::json> const &data,
                                  nlohmann::json const &matcher,
                                  nlohmann::json::json_pointer value_jptr) {
  double acc = 0.;

  if (data.size() == 0) {
    return std::nullopt;
  }

  for (nlohmann::json const &j : data) {
    auto event_it = find_matching(j.begin(), j.end(), matcher);

    if (event_it == j.end()) {
      // TODO: Add warning
      return std::nullopt;
    }

    if (!event_it->contains(value_jptr)) {
      // TODO: Add warning
      return std::nullopt;
    }

    if (!event_it->is_number()) {
      // TODO: Add warning
      return std::nullopt;
    }

    acc += event_it->get<double>();
  }

  return acc / data.size();
}

std::optional<std::string>
get_feature_name(std::vector<nlohmann::json> const &data,
                 nlohmann::json::json_pointer const &name_jptr) {
  if (data.size() == 0) {
    // TODO: Add warning
    return std::nullopt;
  }

  auto const &first_elmt = data[0];
  if (!first_elmt.contains(name_jptr) || !first_elmt.is_string()) {
    // TODO: Add warning
    return std::nullopt;
  }

  return first_elmt;
}

} // namespace grapher

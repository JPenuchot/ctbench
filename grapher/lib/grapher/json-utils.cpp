#include "grapher/json-utils.hpp"
#include "nlohmann/json_fwd.hpp"
#include <llvm/Support/raw_ostream.h>
#include <optional>

namespace grapher {

nlohmann::json::const_iterator
find_matching(nlohmann::json::const_iterator begin,
              nlohmann::json::const_iterator end,
              nlohmann::json const &matcher) {
  nlohmann::json const flat_matcher = matcher.flatten();

  return std::find_if(begin, end, [&](nlohmann::json const &j) -> bool {
    nlohmann::json const flat_j = j.flatten();

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

  for (nlohmann::json const &iteration : data) {

    if (!iteration.contains("traceEvents") ||
        !iteration["traceEvents"].is_array()) {
      llvm::errs()
          << "[WARNING] Invalid iteration data: no traceEvents array field.\n";
      return std::nullopt;
    }

    auto event_it = find_matching(iteration["traceEvents"].begin(),
                                  iteration["traceEvents"].end(), matcher);

    if (event_it == iteration["traceEvents"].end()) {
      llvm::errs()
          << "[WARNING] Invalid iteration data: couldn't match any event.\n";
      return std::nullopt;
    }

    if (!event_it->contains(value_jptr)) {
      llvm::errs() << "[WARNING] Invalid iteration data: no data available at "
                      "given JSON pointer.\n";
      return std::nullopt;
    }

    if (!(*event_it)[value_jptr].is_number()) {
      llvm::errs() << "[WARNING] Invalid iteration data: data at JSON pointer "
                      "isn't a number.\n";
      return std::nullopt;
    }

    acc += event_it->get<double>();
  }

  return acc / data.size();
}

std::optional<std::string>
get_feature_name(benchmark_t const &bench,
                 nlohmann::json::json_pointer const &name_jptr) {
  auto const &[name, entries] = bench;

  if (entries.empty()) {
    llvm::errs() << "[WARNING] Couldn't find feature name for benchmark "
                 << name << ": no entry found.\n";
    return std::nullopt;
  }

  entry_t const &first_entry = *entries.begin();
  if (first_entry.data.empty()) {
    // TODO: Add warning
    llvm::errs() << "[WARNING] Couldn't find feature name for benchmark "
                 << name << ": no iteration in first entry of size "
                 << first_entry.size << ".\n";
    return std::nullopt;
  }

  nlohmann::json const &first_iteration = *first_entry.data.begin();
  if (!first_iteration.contains("/traceEvents/0"_json_pointer)) {
    // TODO: Add warning
    llvm::errs() << "[WARNING] Couldn't find feature name for benchmark "
                 << name << ": first iteration in first entry (size "
                 << first_entry.size << ") has no time trace event.\n";
    return std::nullopt;
  }

  nlohmann::json const &first_event =
      first_iteration["/traceEvents/0"_json_pointer];
  if (!first_event.contains(name_jptr) || !first_event[name_jptr].is_string()) {
    // TODO: Add warning
    llvm::errs() << "[WARNING] Couldn't find feature name for benchmark "
                 << name << ": first time trace event of first entry (size "
                 << first_entry.size << ") has no valid data at JSON pointer "
                 << name_jptr.to_string() << ".\n";
    return std::nullopt;
  }

  return first_event[name_jptr];
}

} // namespace grapher

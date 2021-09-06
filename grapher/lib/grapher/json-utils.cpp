#include "grapher/json-utils.hpp"
#include "nlohmann/json_fwd.hpp"
#include <llvm/Support/raw_ostream.h>
#include <optional>

namespace grapher {

nlohmann::json::const_iterator
find_matching(nlohmann::json::const_iterator begin,
              nlohmann::json::const_iterator end,
              nlohmann::json const &matcher) {
  // Flattening matcher to get a set of (JSON pointer, element)
  nlohmann::json const flat_matcher = matcher.flatten();

  return std::find_if(begin, end, [&](nlohmann::json const &j) -> bool {
    // Flattening current element to get a set of (JSON pointer, element)
    nlohmann::json const flat_j = j.flatten();

    // Checking for flat_matcher's inclusion within flat_j
    for (auto const &[key, value] : flat_matcher.items()) {
      if (auto it = flat_j.find(key); it == flat_j.end() || *it != value) {
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
  int count = 0;

  if (data.size() == 0) {
    return std::nullopt;
  }

  for (nlohmann::json const &iteration : data) {

    if (!iteration.contains("traceEvents") ||
        !iteration["traceEvents"].is_array()) {
      llvm::errs()
          << "[WARNING] Invalid iteration data: no traceEvents array field.\n";
      continue;
    }

    auto event_it = find_matching(iteration["traceEvents"].begin(),
                                  iteration["traceEvents"].end(), matcher);

    if (event_it == iteration["traceEvents"].end()) {
      llvm::errs()
          << "[WARNING] Invalid iteration data: couldn't match any event.\n";
      continue;
    }

    if (!event_it->contains(value_jptr)) {
      llvm::errs() << "[WARNING] Invalid iteration data: no data available at"
                      "given JSON pointer.\n";
      continue;
    }

    if (!(*event_it)[value_jptr].is_number()) {
      llvm::errs() << "[WARNING] Invalid iteration data: data at JSON pointer "
                      "isn't a number.\n";
      continue;
    }

    acc += (*event_it)[value_jptr].get<double>();
    count++;
  }

  if (count == 0) {
    return std::nullopt;
  }

  return acc / count;
}

std::optional<std::string>
get_feature_name(benchmark_t const &bench, nlohmann::json const &matcher,
                 nlohmann::json::json_pointer const &name_jptr) {
  auto const &[name, entries] = bench;

  // Series of data integrity check incoming...

  if (entries.empty()) {
    llvm::errs() << "[WARNING] Couldn't find feature name for benchmark "
                 << name << ": no entry found.\n";
    return std::nullopt;
  }

  entry_t const &first_entry = *entries.begin();
  if (first_entry.data.empty()) {
    llvm::errs() << "[WARNING] Couldn't find feature name for benchmark "
                 << name << ": no iteration in first entry of size "
                 << first_entry.size << ".\n";
    return std::nullopt;
  }

  nlohmann::json const &first_iteration = *first_entry.data.begin();
  if (!first_iteration.contains("traceEvents")) {
    llvm::errs() << "[WARNING] Couldn't find feature name for benchmark "
                 << name << ": no traceEvents field in first entry (size "
                 << first_entry.size << ").\n";
    return std::nullopt;
  }

  nlohmann::json const &trace_events = first_iteration["traceEvents"];
  if (!trace_events.is_array()) {
    llvm::errs() << "[WARNING] Couldn't find feature name for benchmark "
                 << name << ": traceEvents field in first entry (size "
                 << first_entry.size << ") is not an array.\n";
    return std::nullopt;
  }

  nlohmann::json::const_iterator event_it =
      find_matching(trace_events.begin(), trace_events.end(), matcher);
  if (event_it == trace_events.end()) {
    llvm::errs() << "[WARNING] Couldn't find feature name for benchmark "
                 << name << ": no matching even in first entry (size "
                 << first_entry.size << ").\n";
    return std::nullopt;
  }

  nlohmann::json const &event = *event_it;
  if (!event.contains(name_jptr)) {
    llvm::errs() << "[WARNING] Couldn't find feature name for benchmark "
                 << name << ": matched event in first entry (size "
                 << first_entry.size
                 << ") has no data at given name JSON pointer.\n";
    return std::nullopt;
  }

  return event[name_jptr];
}

} // namespace grapher

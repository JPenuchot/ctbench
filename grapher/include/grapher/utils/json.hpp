#pragma once

#include <experimental/source_location>
#include <optional>
#include <string>
#include <type_traits>

#include <llvm/Support/raw_ostream.h>

#include <fmt/core.h>

#include <sciplot/sciplot.hpp>

#include "grapher/core.hpp"
#include "grapher/predicates.hpp"
#include "grapher/utils/error.hpp"

namespace grapher {

/// For each iteration in entry, returns the sum of the values pointed by
/// value_jptr in the events matching the descriptor's predicates.
std::vector<double> get_values(benchmark_iteration_t const &iteration,
                               std::vector<predicate_t> const &predicates,
                               grapher::json_t::json_pointer value_jptr);

/// Merges the contents of b into a, with items of a being overwritten if items
/// present in b share the same key.
grapher::json_t merge_into(grapher::json_t a, grapher::json_t const &b);

/// Generic conversion of a JSON field location to a std::string
template <typename T>
inline std::string field_loc_to_string(T &&field_location) {
  return std::string{std::forward<T>(field_location)};
}

/// Specialized conversion of a JSON field location to a std::string
inline std::string
field_loc_to_string(grapher::json_t::json_pointer const &json_ptr) {
  return json_ptr.to_string();
}

/// Wraps json_t object access with error management.
template <typename LocType>
inline grapher::json_t::const_reference
get_as_json(grapher::json_t const &object, LocType const &field_location,
            const std::experimental::source_location loc =
                std::experimental::source_location::current()) {
  std::string const field_location_str = field_loc_to_string(field_location);

  check(object.contains(field_location),
        fmt::format("Empty field {}:\n{}", field_location_str, object.dump(2)),
        error_v, -1, loc);

  return object[field_location];
}

/// Wraps reference access with type checking and error management.
template <typename ReferenceType, typename LocType>
inline ReferenceType
get_as_ref(grapher::json_t const &object, LocType const &field_location,
           const std::experimental::source_location loc =
               std::experimental::source_location::current()) {

  using ValueType = std::decay_t<ReferenceType>;

  std::string const field_location_str = field_loc_to_string(field_location);

  check(object.contains(field_location),
        fmt::format("Empty field {}:\n{}", field_location_str, object.dump(2)),
        error_v, -1, loc);

  // number_unsigned_t
  if constexpr (std::is_same<grapher::json_t::number_unsigned_t,
                             ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::number_unsigned,
          fmt::format("Invalid field {}, expected unsigned number:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  // number_integer_t
  else if constexpr (std::is_same<grapher::json_t::number_integer_t,
                                  ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::number_integer,
          fmt::format("Invalid field {}, expected integer number:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  // number_float_t
  else if constexpr (std::is_same<grapher::json_t::number_float_t,
                                  ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::number_float,
          fmt::format("Invalid field {}, expected float number:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  // array_t
  else if constexpr (std::is_same<grapher::json_t::array_t, ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::array,
          fmt::format("Invalid field {}, expected array:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  // boolean_t
  else if constexpr (std::is_same<grapher::json_t::boolean_t,
                                  ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::boolean,
          fmt::format("Invalid field {}, expected bool:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  // object_t
  else if constexpr (std::is_same<grapher::json_t::object_t,
                                  ValueType>::value) {
    check(object[field_location].type() == json_t::value_t::object,
          fmt::format("Invalid field {}, expected JSON object:\n{}",
                      field_location_str, object.dump(2)),
          error_v, -1, loc);
  }

  return object[field_location].template get_ref<ReferenceType>();
}

// =============================================================================
// Group descriptors

/// Describes a group of events with a name and a set of constraints. Group
/// descriptors are used by some plotters to target and aggregate trace events
/// for data collection.
///
/// For example, this could be a group descriptor for collecting data about
/// Source trace events that are related to Boost include files:
///
/// \code{.json}
/// {
///   "name": "Boost sources events",
///   "predicates": [
///     {
///       "type": "match",
///       "regex_match": true,
///       "matcher": {
///         "name": "Source",
///         "args": {
///           "details": "/usr/include/boost/*"
///         }
///       }
///     }
///   ]
/// }
/// \endcode
struct group_descriptor_t {
  std::string name;
  grapher::json_t::array_t predicates;
};

/// Returns a default group descriptor as an example.
group_descriptor_t get_default_group_descriptor();

/// Extracts predicates from a group descriptor.
std::vector<predicate_t> get_predicates(group_descriptor_t const &descriptor);

/// Given a group descriptor and a list of events, extracts a list of events
/// that match the group descriptor.
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

// =============================================================================
// Plotter configuration

/// Plot saving helper function. Saves a plot to a given destination. Config may
/// contain parameters such as the export format extensions.
void save_plot(sciplot::Plot2D plot, std::string const &dest,
               grapher::json_t const &config);

/// Returns a default configuration for apply_config.
grapher::json_t base_default_config();

/// Apply config to plot.
sciplot::Plot &apply_config(sciplot::Plot &plot, grapher::json_t const &config);

} // namespace grapher

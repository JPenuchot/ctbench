#include <bits/iterator_concepts.h>
#include <regex>
#include <string>

#include <nlohmann/json.hpp>

#include <llvm/Support/raw_ostream.h>

#include <fmt/core.h>

#include "grapher/core.hpp"
#include "grapher/predicates.hpp"
#include "grapher/utils/error.hpp"
#include "grapher/utils/json.hpp"

namespace grapher::predicates {

/// \ingroup predicates
/// Generates a regex predicate from constraint.
///
/// Example:
/// ```json
/// {
///   "type": "regex",
///   "pointer": "/name",
///   "regex": "Total*"
/// }
/// ```
inline auto regex(grapher::json_t const &constraint) {
  // Validating pointer parameter
  return [pointer =
              grapher::json_t::json_pointer{
                  json_at_ref<json_t::string_t const &>(constraint, "pointer")},
          regex = std::regex(json_at_ref<json_t::string_t const &>(
              constraint, "regex"))](grapher::json_t const &value) -> bool {
    if (!value.contains(pointer) || !value[pointer].is_string()) {
      return false;
    }

    return std::regex_match(
        json_at_ref<json_t::string_t const &>(value, pointer), regex);
  };
}

/// \ingroup predicates
/// Generates a match predicate from constraint. A match predicate returns true
/// if all the fields contained in the matcher object exist in the observed
/// value and hold the same values.
///
/// String values will be regex matched if "regex_match" option is set to true.
///
/// Examples:
///
/// Using regex match:
/// ```json
/// {
///   "type": "match",
///   "regex_match": true,
///   "matcher":
///   {
///     "name": "Source",
///     "args":
///     {
///       "details": "/usr/include/boost/*"
///     }
///   }
/// }
/// ```
///
/// Not using regex match:
/// ```json
/// {
///   "type": "match",
///   "regex_match": false,
///   "matcher":
///   {
///     "name": "Source",
///     "args":
///     {
///       "details": "/usr/include/boost/preprocessor/logical/bool.hpp"
///     }
///   }
/// }
/// ```
inline auto match(grapher::json_t const &constraint) {
  return [matcher_flat = json_at(constraint, "matcher").flatten(),
          regex_match_opt = constraint.value("regex", false)](
             grapher::json_t const &value) -> bool {
    auto items_iteration_proxy = matcher_flat.items();
    return std::all_of(
        items_iteration_proxy.begin(), items_iteration_proxy.end(),
        [&](auto const &matcher_item_kv) -> bool {
          // Pointer to the value we should observe
          grapher::json_t::json_pointer const ptr(matcher_item_kv.key());

          // Checking for existence of matching value
          if (!value.contains(ptr)) {
            return false;
          }

          // Regex match
          if (regex_match_opt) {
            return std::regex_match(to_string(value[ptr]),
                                    std::regex(matcher_item_kv.value()));
          }

          // Regular match
          return value[ptr] == matcher_item_kv.value();
        });
  };
}

/// \ingroup predicates
/// Generates a streq predicate from constraint.
///
/// Example:
/// ```json
/// {
///   "type": "streq",
///   "pointer": "/name",
///   "string": "Total Source"
/// }
/// ```
inline auto streq(grapher::json_t const &constraint) {
  return [pointer =
              grapher::json_t::json_pointer{
                  json_at_ref<json_t::string_t const &>(constraint, "pointer")},
          str = json_at_ref<json_t::string_t const &>(constraint, "string")](
             grapher::json_t const &value) -> bool {
    if (!value.contains(pointer) || !value[pointer].is_string()) {
      return false;
    }

    return value[pointer].get_ref<grapher::json_t::string_t const &>() == str;
  };
}

/// \ingroup predicates
/// Satisfied if one of the predicates in the first or second field is
/// satisfied.
///
/// Example:
/// ```json
/// {
///   "type": "op_or",
///   "first":
///   {
///     "type": "regex",
///     "pointer": "/name",
///     "regex": "Total*"
///   },
///   "second":
///   {
///     "type": "streq",
///     "pointer": "/name",
///     "string": "Source"
///   }
/// }
/// ```
inline auto op_or(grapher::json_t const &constraint) {
  return [first = get_predicate(json_at(constraint, "first")),
          second = get_predicate(json_at(constraint, "second"))](
             grapher::json_t const &value) -> bool {
    return first(value) || second(value);
  };
}

/// \ingroup predicates
/// Satisfied if both predicates in the first and second field are satisfied.
///
/// Example:
/// ```json
/// {
///   "type": "op_and",
///   "first":
///   {
///     "type": "streq",
///     "pointer": "/name",
///     "string": "Total Source"
///   },
///   "second":
///   {
///     "type": "streq",
///     "pointer": "/name",
///     "string": "Total InstantiateClass"
///   }
/// }
/// ```
inline auto op_and(grapher::json_t const &constraint) {
  return [first = get_predicate(json_at(constraint, "first")),
          second = get_predicate(json_at(constraint, "second"))](
             grapher::json_t const &value) -> bool {
    return first(value) && second(value);
  };
}

/// \ingroup predicates
/// Predicate that is always true.
///
/// Example:
/// ```json
/// {
///   "type": "val_true",
/// }
/// ```
inline auto val_true(grapher::json_t const & /* unused */) {
  return [](grapher::json_t const &) -> bool { return true; };
}

/// \ingroup predicates
/// Predicate that is always false.
///
/// Example:
/// ```json
/// {
///   "type": "val_false",
/// }
/// ```
inline auto val_false(grapher::json_t const & /* unused */) {
  return [](grapher::json_t const &) -> bool { return false; };
}

} // namespace grapher::predicates

namespace grapher {

/// \ingroup predicates
/// Builds predicate and stores it in an std::function object.
predicate_t get_predicate(grapher::json_t const &constraint) {
  std::string constraint_type =
      json_at_ref<json_t::string_t const &>(constraint, "type");

#define REGISTER_PREDICATE(name)                                               \
  if (constraint_type == #name) {                                              \
    return predicates::name(constraint);                                       \
  }

  REGISTER_PREDICATE(regex);
  REGISTER_PREDICATE(match);
  REGISTER_PREDICATE(streq);
  REGISTER_PREDICATE(op_or);
  REGISTER_PREDICATE(op_and);
  REGISTER_PREDICATE(val_true);
  REGISTER_PREDICATE(val_false);

#undef REGISTER_PREDICATE

  check(false,
        fmt::format("Predicate error, invalid type:\n{}", constraint.dump(2)));
  return {};
}

} // namespace grapher

#include "grapher/predicates.hpp"
#include "grapher/utils/json.hpp"

#include <bits/ranges_algo.h>
#include <nlohmann/json_fwd.hpp>
#include <regex>
#include <string>

#include <nlohmann/json.hpp>

#include <llvm/Support/raw_ostream.h>

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
inline auto regex(nlohmann::json const &constraint) {
  // Validating pointer parameter
  return [pointer = nlohmann::json::json_pointer{json_value<std::string>(
              constraint, "pointer")},
          regex = std::regex(json_value<std::string>(constraint, "regex"))](
             nlohmann::json const &value) -> bool {
    if (!value.contains(pointer) || !value[pointer].is_string()) {
      return false;
    }

    return std::regex_match(json_value<std::string>(value, pointer), regex);
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

inline auto match(nlohmann::json const &constraint) {
  return [matcher_flat =
              json_value<nlohmann::json>(constraint, "matcher").flatten(),
          regex_match_opt = constraint.value("regex", false)](
             nlohmann::json const &value) -> bool {
    auto items_iteration_proxy = matcher_flat.items();
    return std::all_of(
        items_iteration_proxy.begin(), items_iteration_proxy.end(),
        [&](auto const &matcher_item_kv) -> bool {
          // Pointer to the value we should observe
          nlohmann::json::json_pointer const ptr(matcher_item_kv.key());

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
inline auto streq(nlohmann::json const &constraint) {
  return [pointer = nlohmann::json::json_pointer{json_value<std::string>(
              constraint, "pointer")},
          str = json_value<std::string>(constraint, "string")](
             nlohmann::json const &value) -> bool {
    if (!value.contains(pointer) || !value[pointer].is_string()) {
      return false;
    }

    return value[pointer].get_ref<nlohmann::json::string_t const &>() == str;
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
inline auto op_or(nlohmann::json const &constraint) {
  return
      [first = get_predicate(json_value<nlohmann::json>(constraint, "first")),
       second = get_predicate(json_value<nlohmann::json>(
           constraint, "second"))](nlohmann::json const &value) -> bool {
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
inline auto op_and(nlohmann::json const &constraint) {
  return
      [first = get_predicate(json_value<nlohmann::json>(constraint, "first")),
       second = get_predicate(json_value<nlohmann::json>(
           constraint, "second"))](nlohmann::json const &value) -> bool {
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
inline auto val_true(nlohmann::json const & /* unused */) {
  return [](nlohmann::json const &) -> bool { return true; };
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
inline auto val_false(nlohmann::json const & /* unused */) {
  return [](nlohmann::json const &) -> bool { return false; };
}

} // namespace grapher::predicates

namespace grapher {

/// \ingroup predicates
/// Builds predicate and stores it in an std::function object.
predicate_t get_predicate(nlohmann::json const &constraint) {
  std::string constraint_type = json_value<std::string>(constraint, "type");

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

  llvm::errs() << "[ERROR] Predicate error, invalid type:\n"
               << constraint.dump(2) << '\n';
  std::exit(1);
}

} // namespace grapher

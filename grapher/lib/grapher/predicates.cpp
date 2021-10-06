#include "grapher/predicates.hpp"
#include "grapher/utils/json.hpp"

#include <nlohmann/json_fwd.hpp>
#include <regex>
#include <string>

#include <nlohmann/json.hpp>

#include <llvm/Support/raw_ostream.h>

namespace grapher {
predicate_t get_predicate(nlohmann::json const &constraint);
}

namespace grapher::predicates {

/// \defgroup predicates
/// Predicates for group descriptors.

/// \ingroup predicates
/// Generates a regex predicate from constraint.
inline auto regex(nlohmann::json const &constraint) {
  // Validating pointer parameter
  return [pointer = nlohmann::json::json_pointer{json_value<std::string>(
              constraint, "pointer")},
          regex = json_value<std::string>(constraint, "regex")](
             nlohmann::json const &value) -> bool {
    if (!value.contains(pointer) || !value[pointer].is_string()) {
      return false;
    }

    return std::regex_match(json_value<std::string>(value, pointer),
                            std::regex(regex.data()));
  };
}

/// \ingroup predicates
/// Generates a match predicate from constraint.
inline auto match(nlohmann::json const &constraint) {
  return [pointer = nlohmann::json::json_pointer(
              json_value<std::string>(constraint, "pointer")),
          matcher_flat =
              json_value<nlohmann::json>(constraint, "matcher").flatten()](
             nlohmann::json const &value) -> bool {
    for (auto const &[k, v] : matcher_flat.items()) {
      if (value[nlohmann::json::json_pointer(k)] != v) {
        return false;
      }
    }
    return true;
  };
}

/// \ingroup predicates
/// Generates a streq predicate from constraint.
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
inline auto op_and(nlohmann::json const &constraint) {
  return
      [first = get_predicate(json_value<nlohmann::json>(constraint, "first")),
       second = get_predicate(json_value<nlohmann::json>(
           constraint, "second"))](nlohmann::json const &value) -> bool {
        return first(value) && second(value);
      };
}

inline auto val_true(nlohmann::json const &) {
  return [](nlohmann::json const &) -> bool { return true; };
}

inline auto val_false(nlohmann::json const &) {
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

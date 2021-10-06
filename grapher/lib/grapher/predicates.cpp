#include "grapher/predicates.hpp"
#include "grapher/utils/json.hpp"

#include <regex>

#include <nlohmann/json.hpp>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/raw_ostream.h>
#include <string>

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
  // Validating pointer parameter
  if (!constraint.contains("pointer") || !constraint["pointer"].is_string()) {
    llvm::errs() << "[ERROR] Regex constraint has no pointer string value:\n"
                 << constraint.dump(2) << '\n';
    std::exit(1);
  }

  // Validating string parameter
  if (!constraint.contains("string") || !constraint["string"].is_string()) {
    llvm::errs() << "[ERROR] Streq constraint has no valid \"string\" value:\n"
                 << constraint.dump(2) << '\n';
    std::exit(1);
  }

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

} // namespace grapher::predicates

namespace grapher {

/// \ingroup predicates
/// Builds predicate and stores it in an std::function object.
predicate_t get_predicate(nlohmann::json const &constraint) {
  if (!constraint.contains("type") || !constraint["type"].is_string()) {
    llvm::errs() << "[ERROR] Constraint has no type: \n"
                 << constraint.dump(2) << '\n';
    std::exit(1);
  }

  llvm::SmallString<8> constraint_type(constraint["type"]);

  if (constraint_type == "regex") {
    return predicates::regex(constraint);
  }
  if (constraint_type == "match") {
    return predicates::match(constraint);
  }
  if (constraint_type == "streq") {
    return predicates::streq(constraint);
  }

  llvm::errs() << "[ERROR] Invalid constraint type:\n"
               << constraint.dump(2) << '\n';
  std::exit(1);
}

} // namespace grapher

#pragma once

// STL
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

// External
#include <nlohmann/json.hpp>

namespace grapher {

/// Raw entry (before proper extraction in an entry_t type)
using raw_entry_t = std::tuple<std::filesystem::path, nlohmann::json>;

/// Measure type
using measure_t = long;

/// Contains all the metrics for a given benchmark
struct entry_t {
  int size;

  measure_t execute_compiler;
  measure_t frontend;
  measure_t source;
  measure_t instantiate_function;
  measure_t parse_class;
  measure_t instantiate_class;
  measure_t backend;
  measure_t opt_module;
  measure_t parse_template;
  measure_t opt_function;
  measure_t run_pass;
  measure_t per_module_passes;
  measure_t perform_pending_instantiations;
  measure_t run_loop_pass;
  measure_t code_gen_passes;
  measure_t code_gen_function;
  measure_t per_function_passes;
};

/// Represents a kind of measure
enum measure_kind_t {
  execute_compiler_v,
  frontend_v,
  source_v,
  instantiate_function_v,
  parse_class_v,
  instantiate_class_v,
  backend_v,
  opt_module_v,
  parse_template_v,
  opt_function_v,
  run_pass_v,
  per_module_passes_v,
  perform_pending_instantiations_v,
  run_loop_pass_v,
  code_gen_passes_v,
  code_gen_function_v,
  per_function_passes_v,
};

/// Collection of measures to iterate on
auto constexpr measure_kind_iterable = {
    execute_compiler_v,
    frontend_v,
    source_v,
    instantiate_function_v,
    parse_class_v,
    instantiate_class_v,
    backend_v,
    opt_module_v,
    parse_template_v,
    opt_function_v,
    run_pass_v,
    per_module_passes_v,
    perform_pending_instantiations_v,
    run_loop_pass_v,
    code_gen_passes_v,
    code_gen_function_v,
    per_function_passes_v,
};

/// Get the measure designated by m in e
constexpr measure_t get_measure(entry_t const &e, measure_kind_t m) {
  switch (m) {
  case execute_compiler_v:
    return e.execute_compiler;
  case frontend_v:
    return e.frontend;
  case source_v:
    return e.source;
  case instantiate_function_v:
    return e.instantiate_function;
  case parse_class_v:
    return e.parse_class;
  case instantiate_class_v:
    return e.instantiate_class;
  case backend_v:
    return e.backend;
  case opt_module_v:
    return e.opt_module;
  case parse_template_v:
    return e.parse_template;
  case opt_function_v:
    return e.opt_function;
  case run_pass_v:
    return e.run_pass;
  case per_module_passes_v:
    return e.per_module_passes;
  case perform_pending_instantiations_v:
    return e.perform_pending_instantiations;
  case run_loop_pass_v:
    return e.run_loop_pass;
  case code_gen_passes_v:
    return e.code_gen_passes;
  case code_gen_function_v:
    return e.code_gen_function;
  case per_function_passes_v:
    return e.per_function_passes;
  }
  return -1;
}

/// Get the measure name of m in a string_view
constexpr std::string_view get_measure_name(measure_kind_t m) {
  switch (m) {
  case execute_compiler_v:
    return "Execute Compiler";
  case frontend_v:
    return "Frontend";
  case source_v:
    return "Source";
  case instantiate_function_v:
    return "Instantiate Function";
  case parse_class_v:
    return "Parse Class";
  case instantiate_class_v:
    return "Instantiate Class";
  case backend_v:
    return "Backend";
  case opt_module_v:
    return "Opt Module";
  case parse_template_v:
    return "Parse Template";
  case opt_function_v:
    return "Opt Function";
  case run_pass_v:
    return "Run Pass";
  case per_module_passes_v:
    return "Per Module Passes";
  case perform_pending_instantiations_v:
    return "Perform Pending Instantiations";
  case run_loop_pass_v:
    return "Run Loop Pass";
  case code_gen_passes_v:
    return "Code Gen Passes";
  case code_gen_function_v:
    return "Code Gen Function";
  case per_function_passes_v:
    return "Per Function Passes";
  }
  return "Invalid";
}

/// Benchmark category
using benchmark_t = std::tuple<std::string, std::vector<entry_t>>;

/// Benchmark categories
using category_t = std::vector<benchmark_t>;

} // namespace grapher

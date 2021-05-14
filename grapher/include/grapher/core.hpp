#pragma once

/// \file
/// Core data types for benchmark representation.

#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

namespace grapher {

/// Measure type
using measure_t = long;

/// Contains all the metrics for a given benchmark
struct entry_t {
  static constexpr int nsize = -1;

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

  // Check entry validity
  inline operator bool() const noexcept { return size != nsize; }
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

/// Manages benchmark feature storage in separate vectors,
/// and provides easy ways to access them.
class entry_storage_t {
  std::size_t _size;

  std::vector<int> _size_v;
  std::vector<measure_t> _execute_compiler_v;
  std::vector<measure_t> _frontend_v;
  std::vector<measure_t> _source_v;
  std::vector<measure_t> _instantiate_function_v;
  std::vector<measure_t> _parse_class_v;
  std::vector<measure_t> _instantiate_class_v;
  std::vector<measure_t> _backend_v;
  std::vector<measure_t> _opt_module_v;
  std::vector<measure_t> _parse_template_v;
  std::vector<measure_t> _opt_function_v;
  std::vector<measure_t> _run_pass_v;
  std::vector<measure_t> _per_module_passes_v;
  std::vector<measure_t> _perform_pending_instantiations_v;
  std::vector<measure_t> _run_loop_pass_v;
  std::vector<measure_t> _code_gen_passes_v;
  std::vector<measure_t> _code_gen_function_v;
  std::vector<measure_t> _per_function_passes_v;

public:
  entry_storage_t(std::size_t n)
      : _size(n), _size_v(n), _execute_compiler_v(n), _frontend_v(n),
        _source_v(n), _instantiate_function_v(n), _parse_class_v(n),
        _instantiate_class_v(n), _backend_v(n), _opt_module_v(n),
        _parse_template_v(n), _opt_function_v(n), _run_pass_v(n),
        _per_module_passes_v(n), _perform_pending_instantiations_v(n),
        _run_loop_pass_v(n), _code_gen_passes_v(n), _code_gen_function_v(n),
        _per_function_passes_v(n) {}

  entry_storage_t(std::vector<entry_t> const &v) : entry_storage_t(v.size()) {
    for (std::size_t i = 0; i < _size; i++)
      store(v[i], i);
  }

  std::size_t size() const noexcept { return _size; }

  //============================================================================
  // Load/Store

  entry_t load(std::size_t i) const {
    return entry_t{_size_v[i],
                   _execute_compiler_v[i],
                   _frontend_v[i],
                   _source_v[i],
                   _instantiate_function_v[i],
                   _parse_class_v[i],
                   _instantiate_class_v[i],
                   _backend_v[i],
                   _opt_module_v[i],
                   _parse_template_v[i],
                   _opt_function_v[i],
                   _run_pass_v[i],
                   _per_module_passes_v[i],
                   _perform_pending_instantiations_v[i],
                   _run_loop_pass_v[i],
                   _code_gen_passes_v[i],
                   _code_gen_function_v[i],
                   _per_function_passes_v[i]};
  }

  entry_storage_t &store(entry_t const &e, std::size_t i) {
    _size_v[i] = e.size;
    _execute_compiler_v[i] = e.execute_compiler;
    _frontend_v[i] = e.frontend;
    _source_v[i] = e.source;
    _instantiate_function_v[i] = e.instantiate_function;
    _parse_class_v[i] = e.parse_class;
    _instantiate_class_v[i] = e.instantiate_class;
    _backend_v[i] = e.backend;
    _opt_module_v[i] = e.opt_module;
    _parse_template_v[i] = e.parse_template;
    _opt_function_v[i] = e.opt_function;
    _run_pass_v[i] = e.run_pass;
    _per_module_passes_v[i] = e.per_module_passes;
    _perform_pending_instantiations_v[i] = e.perform_pending_instantiations;
    _run_loop_pass_v[i] = e.run_loop_pass;
    _code_gen_passes_v[i] = e.code_gen_passes;
    _code_gen_function_v[i] = e.code_gen_function;
    _per_function_passes_v[i] = e.per_function_passes;
    return *this;
  }

  //============================================================================
  // Content access

  // Compile-time

  /// Returns a reference to a given measure for the element of index i.
  template <measure_kind_t M> auto &get_measure(std::size_t i) noexcept {
    if constexpr (M == execute_compiler_v) {
      return _execute_compiler_v[i];
    }
    if constexpr (M == frontend_v) {
      return _frontend_v[i];
    }
    if constexpr (M == source_v) {
      return _source_v[i];
    }
    if constexpr (M == instantiate_function_v) {
      return _instantiate_function_v[i];
    }
    if constexpr (M == parse_class_v) {
      return _parse_class_v[i];
    }
    if constexpr (M == instantiate_class_v) {
      return _instantiate_class_v[i];
    }
    if constexpr (M == backend_v) {
      return _backend_v[i];
    }
    if constexpr (M == opt_module_v) {
      return _opt_module_v[i];
    }
    if constexpr (M == parse_template_v) {
      return _parse_template_v[i];
    }
    if constexpr (M == opt_function_v) {
      return _opt_function_v[i];
    }
    if constexpr (M == run_pass_v) {
      return _run_pass_v[i];
    }
    if constexpr (M == per_module_passes_v) {
      return _per_module_passes_v[i];
    }
    if constexpr (M == perform_pending_instantiations_v) {
      return _perform_pending_instantiations_v[i];
    }
    if constexpr (M == run_loop_pass_v) {
      return _run_loop_pass_v[i];
    }
    if constexpr (M == code_gen_passes_v) {
      return _code_gen_passes_v[i];
    }
    if constexpr (M == code_gen_function_v) {
      return _code_gen_function_v[i];
    }
    if constexpr (M == per_function_passes_v) {
      return _per_function_passes_v[i];
    }
  }

  /// Returns a given measure for the element of index i.
  template <measure_kind_t M> auto get_measure(std::size_t i) const noexcept {
    if constexpr (M == execute_compiler_v) {
      return _execute_compiler_v[i];
    }
    if constexpr (M == frontend_v) {
      return _frontend_v[i];
    }
    if constexpr (M == source_v) {
      return _source_v[i];
    }
    if constexpr (M == instantiate_function_v) {
      return _instantiate_function_v[i];
    }
    if constexpr (M == parse_class_v) {
      return _parse_class_v[i];
    }
    if constexpr (M == instantiate_class_v) {
      return _instantiate_class_v[i];
    }
    if constexpr (M == backend_v) {
      return _backend_v[i];
    }
    if constexpr (M == opt_module_v) {
      return _opt_module_v[i];
    }
    if constexpr (M == parse_template_v) {
      return _parse_template_v[i];
    }
    if constexpr (M == opt_function_v) {
      return _opt_function_v[i];
    }
    if constexpr (M == run_pass_v) {
      return _run_pass_v[i];
    }
    if constexpr (M == per_module_passes_v) {
      return _per_module_passes_v[i];
    }
    if constexpr (M == perform_pending_instantiations_v) {
      return _perform_pending_instantiations_v[i];
    }
    if constexpr (M == run_loop_pass_v) {
      return _run_loop_pass_v[i];
    }
    if constexpr (M == code_gen_passes_v) {
      return _code_gen_passes_v[i];
    }
    if constexpr (M == code_gen_function_v) {
      return _code_gen_function_v[i];
    }
    if constexpr (M == per_function_passes_v) {
      return _per_function_passes_v[i];
    }
  }

  // Runtime

  /// Returns a reference to a given measure for the element of index i.
  measure_t &get_measure(std::size_t i, measure_kind_t m) noexcept {
    switch (m) {
    case execute_compiler_v:
      return _execute_compiler_v[i];
    case frontend_v:
      return _frontend_v[i];
    case source_v:
      return _source_v[i];
    case instantiate_function_v:
      return _instantiate_function_v[i];
    case parse_class_v:
      return _parse_class_v[i];
    case instantiate_class_v:
      return _instantiate_class_v[i];
    case backend_v:
      return _backend_v[i];
    case opt_module_v:
      return _opt_module_v[i];
    case parse_template_v:
      return _parse_template_v[i];
    case opt_function_v:
      return _opt_function_v[i];
    case run_pass_v:
      return _run_pass_v[i];
    case per_module_passes_v:
      return _per_module_passes_v[i];
    case perform_pending_instantiations_v:
      return _perform_pending_instantiations_v[i];
    case run_loop_pass_v:
      return _run_loop_pass_v[i];
    case code_gen_passes_v:
      return _code_gen_passes_v[i];
    case code_gen_function_v:
      return _code_gen_function_v[i];
    case per_function_passes_v:
      return _per_function_passes_v[i];
    };
  }

  /// Returns a given measure for the element of index i.
  measure_t get_measure(std::size_t i, measure_kind_t m) const noexcept {
    switch (m) {
    case execute_compiler_v:
      return _execute_compiler_v[i];
    case frontend_v:
      return _frontend_v[i];
    case source_v:
      return _source_v[i];
    case instantiate_function_v:
      return _instantiate_function_v[i];
    case parse_class_v:
      return _parse_class_v[i];
    case instantiate_class_v:
      return _instantiate_class_v[i];
    case backend_v:
      return _backend_v[i];
    case opt_module_v:
      return _opt_module_v[i];
    case parse_template_v:
      return _parse_template_v[i];
    case opt_function_v:
      return _opt_function_v[i];
    case run_pass_v:
      return _run_pass_v[i];
    case per_module_passes_v:
      return _per_module_passes_v[i];
    case perform_pending_instantiations_v:
      return _perform_pending_instantiations_v[i];
    case run_loop_pass_v:
      return _run_loop_pass_v[i];
    case code_gen_passes_v:
      return _code_gen_passes_v[i];
    case code_gen_function_v:
      return _code_gen_function_v[i];
    case per_function_passes_v:
      return _per_function_passes_v[i];
    }
  }
};

class benchmark_t {
private:
  std::string _name;
  std::size_t _size;
  std::size_t _iterations;
  entry_storage_t _entries;

public:
  benchmark_t(std::string name, std::size_t size,
              std::size_t iterations) noexcept
      : _name(std::move(name)), _size(size), _iterations(iterations),
        _entries(size * iterations) {}

  //============================================================================
  // Props

  /// Returns the name of the benchmark.
  inline std::string const &name() const noexcept { return _name; }

  /// Returns the number of different sizes of the benchmark.
  inline std::size_t size() const noexcept { return _size; }

  /// Returns the number of iterations per size of the benchmark.
  inline std::size_t iterations() const noexcept { return _iterations; }

  //============================================================================
  // Content access

  /// Loads an entry_t value for a given size index, for a given iteration.
  inline entry_t load(std::size_t size_id, std::size_t iteration) const {
    return _entries.load(size_id * _iterations + iteration);
  }


  /// Stores an entry_t value at a given size index, for a given iteration.
  benchmark_t &store(entry_t const &e, std::size_t size_id,
                     std::size_t iteration) {
    _entries.store(e, size_id * _iterations + iteration);
    return *this;
  }

  template <measure_kind_t M>
  inline auto &get(std::size_t size_id, std::size_t iteration) noexcept {
    return _entries.get_measure<M>(size_id * _iterations + iteration);
  }

  template <measure_kind_t M>
  inline auto get(std::size_t size_id, std::size_t iteration) const noexcept {
    return _entries.get_measure<M>(size_id * _iterations + iteration);
  }

  inline auto &get(std::size_t size_id, std::size_t iteration,
                   measure_kind_t m) noexcept {
    return _entries.get_measure(size_id * _iterations + iteration, m);
  }

  inline auto get(std::size_t size_id, std::size_t iteration,
                  measure_kind_t m) const noexcept {
    return _entries.get_measure(size_id * _iterations + iteration, m);
  }
};

/// Benchmark categories
using category_t = std::vector<benchmark_t>;

} // namespace grapher

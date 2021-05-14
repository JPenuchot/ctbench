#pragma once

#include <grapher/core.hpp>

namespace grapher {

/// SoA storage for a fixed amount of benchmark entries.
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

  //============================================================================
  // Storage access (internal use only)

  inline std::vector<int> &get_size_storage() noexcept { return _size_v; }

  inline std::vector<int> const &get_size_storage() const noexcept {
    return _size_v;
  }

  template <measure_kind_t M>
  inline std::vector<measure_t> &get_measure_storage() noexcept {
    if constexpr (M == execute_compiler_v) {
      return _execute_compiler_v;
    }
    if constexpr (M == frontend_v) {
      return _frontend_v;
    }
    if constexpr (M == source_v) {
      return _source_v;
    }
    if constexpr (M == instantiate_function_v) {
      return _instantiate_function_v;
    }
    if constexpr (M == parse_class_v) {
      return _parse_class_v;
    }
    if constexpr (M == instantiate_class_v) {
      return _instantiate_class_v;
    }
    if constexpr (M == backend_v) {
      return _backend_v;
    }
    if constexpr (M == opt_module_v) {
      return _opt_module_v;
    }
    if constexpr (M == parse_template_v) {
      return _parse_template_v;
    }
    if constexpr (M == opt_function_v) {
      return _opt_function_v;
    }
    if constexpr (M == run_pass_v) {
      return _run_pass_v;
    }
    if constexpr (M == per_module_passes_v) {
      return _per_module_passes_v;
    }
    if constexpr (M == perform_pending_instantiations_v) {
      return _perform_pending_instantiations_v;
    }
    if constexpr (M == run_loop_pass_v) {
      return _run_loop_pass_v;
    }
    if constexpr (M == code_gen_passes_v) {
      return _code_gen_passes_v;
    }
    if constexpr (M == code_gen_function_v) {
      return _code_gen_function_v;
    }
    if constexpr (M == per_function_passes_v) {
      return _per_function_passes_v;
    }
  }

  template <measure_kind_t M>
  inline std::vector<measure_t> const &get_measure_storage() const noexcept {
    if constexpr (M == execute_compiler_v) {
      return _execute_compiler_v;
    }
    if constexpr (M == frontend_v) {
      return _frontend_v;
    }
    if constexpr (M == source_v) {
      return _source_v;
    }
    if constexpr (M == instantiate_function_v) {
      return _instantiate_function_v;
    }
    if constexpr (M == parse_class_v) {
      return _parse_class_v;
    }
    if constexpr (M == instantiate_class_v) {
      return _instantiate_class_v;
    }
    if constexpr (M == backend_v) {
      return _backend_v;
    }
    if constexpr (M == opt_module_v) {
      return _opt_module_v;
    }
    if constexpr (M == parse_template_v) {
      return _parse_template_v;
    }
    if constexpr (M == opt_function_v) {
      return _opt_function_v;
    }
    if constexpr (M == run_pass_v) {
      return _run_pass_v;
    }
    if constexpr (M == per_module_passes_v) {
      return _per_module_passes_v;
    }
    if constexpr (M == perform_pending_instantiations_v) {
      return _perform_pending_instantiations_v;
    }
    if constexpr (M == run_loop_pass_v) {
      return _run_loop_pass_v;
    }
    if constexpr (M == code_gen_passes_v) {
      return _code_gen_passes_v;
    }
    if constexpr (M == code_gen_function_v) {
      return _code_gen_function_v;
    }
    if constexpr (M == per_function_passes_v) {
      return _per_function_passes_v;
    }
  }

  inline std::vector<measure_t> &
  get_measure_storage(measure_kind_t m) noexcept {
    switch (m) {
    case execute_compiler_v:
      return _execute_compiler_v;
    case frontend_v:
      return _frontend_v;
    case source_v:
      return _source_v;
    case instantiate_function_v:
      return _instantiate_function_v;
    case parse_class_v:
      return _parse_class_v;
    case instantiate_class_v:
      return _instantiate_class_v;
    case backend_v:
      return _backend_v;
    case opt_module_v:
      return _opt_module_v;
    case parse_template_v:
      return _parse_template_v;
    case opt_function_v:
      return _opt_function_v;
    case run_pass_v:
      return _run_pass_v;
    case per_module_passes_v:
      return _per_module_passes_v;
    case perform_pending_instantiations_v:
      return _perform_pending_instantiations_v;
    case run_loop_pass_v:
      return _run_loop_pass_v;
    case code_gen_passes_v:
      return _code_gen_passes_v;
    case code_gen_function_v:
      return _code_gen_function_v;
    case per_function_passes_v:
      return _per_function_passes_v;
    };
  }

  inline std::vector<measure_t> const &
  get_measure_storage(measure_kind_t m) const noexcept {
    switch (m) {
    case execute_compiler_v:
      return _execute_compiler_v;
    case frontend_v:
      return _frontend_v;
    case source_v:
      return _source_v;
    case instantiate_function_v:
      return _instantiate_function_v;
    case parse_class_v:
      return _parse_class_v;
    case instantiate_class_v:
      return _instantiate_class_v;
    case backend_v:
      return _backend_v;
    case opt_module_v:
      return _opt_module_v;
    case parse_template_v:
      return _parse_template_v;
    case opt_function_v:
      return _opt_function_v;
    case run_pass_v:
      return _run_pass_v;
    case per_module_passes_v:
      return _per_module_passes_v;
    case perform_pending_instantiations_v:
      return _perform_pending_instantiations_v;
    case run_loop_pass_v:
      return _run_loop_pass_v;
    case code_gen_passes_v:
      return _code_gen_passes_v;
    case code_gen_function_v:
      return _code_gen_function_v;
    case per_function_passes_v:
      return _per_function_passes_v;
    };
  }

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

  /// Returns a reference to the measure for the element of index i.
  template <measure_kind_t M> auto &get_measure(std::size_t i) noexcept {
    return get_measure_storage<M>()[i];
  }

  /// Returns the measure for the element of index i.
  template <measure_kind_t M> auto get_measure(std::size_t i) const noexcept {
    return get_measure_storage<M>()[i];
  }

  // Runtime

  /// Returns a reference to the measure for the element of index i.
  measure_t &get_measure(std::size_t i, measure_kind_t m) noexcept {
    return get_measure_storage(m)[i];
  }

  /// Returns a given measure for the element of index i.
  measure_t get_measure(std::size_t i, measure_kind_t m) const noexcept {
    return get_measure_storage(m)[i];
  }

  /// Returns the size of a benchmark at given index.
  inline int get_size(std::size_t i) const noexcept {
    return get_size_storage()[i];
  }

  /// Returns a reference to size of a benchmark at given index.
  inline int &get_size(std::size_t i) noexcept { return get_size_storage()[i]; }

  //============================================================================
  // Iterators

  template <measure_kind_t M> auto measure_begin() noexcept {
    return get_measure_storage<M>().begin();
  }

  template <measure_kind_t M> auto measure_begin() const noexcept {
    return get_measure_storage<M>().cbegin();
  }

  template <measure_kind_t M> auto measure_end() noexcept {
    return get_measure_storage<M>().end();
  }

  template <measure_kind_t M> auto measure_end() const noexcept {
    return get_measure_storage<M>().cend();
  }

  auto size_begin() noexcept { return get_size_storage().begin(); }

  auto size_begin() const noexcept { return get_size_storage().cbegin(); }

  auto size_end() noexcept { return get_size_storage().end(); }

  auto size_end() const noexcept { return get_size_storage().cend(); }
};

/// Data structure for benchmarks. A benchmark consists in a fixed amount of
/// iterations of several benchmark cases of various sizes. benchmark_t stores
/// entries using entry_storage_t.
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

  /// Loads an entry_t value for a given size index and iteration.
  inline entry_t load(std::size_t size_id, std::size_t iteration) const {
    return _entries.load(size_id * _iterations + iteration);
  }

  /// Stores an entry_t value at a given size index and iteration.
  benchmark_t &store(entry_t const &e, std::size_t size_id,
                     std::size_t iteration) {
    _entries.store(e, size_id * _iterations + iteration);
    return *this;
  }

  /// Returns a reference to the measure at a given size index and iteration.
  template <measure_kind_t M>
  inline auto &get_measure(std::size_t size_id,
                           std::size_t iteration) noexcept {
    return _entries.get_measure<M>(size_id * _iterations + iteration);
  }

  /// Returns the measure at a given size index and iteration.
  template <measure_kind_t M>
  inline auto get_measure(std::size_t size_id,
                          std::size_t iteration) const noexcept {
    return _entries.get_measure<M>(size_id * _iterations + iteration);
  }

  /// Returns a reference to the measure at a given size index and iteration.
  inline auto &get_measure(std::size_t size_id, std::size_t iteration,
                           measure_kind_t m) noexcept {
    return _entries.get_measure(size_id * _iterations + iteration, m);
  }

  /// Returns the measure at a given size index and iteration.
  inline auto get_measure(std::size_t size_id, std::size_t iteration,
                          measure_kind_t m) const noexcept {
    return _entries.get_measure(size_id * _iterations + iteration, m);
  }

  // Returns a reference to the size of an entry at a given size index and
  // iteration.
  inline auto &get_size(std::size_t size_id, std::size_t iteration) noexcept {
    return _entries.get_size(size_id * _iterations + iteration);
  }

  // Returns the size of an entry at a given size index and iteration.
  inline auto get_size(std::size_t size_id,
                       std::size_t iteration) const noexcept {
    return _entries.get_size(size_id * _iterations + iteration);
  }
};

/// Benchmark categories
using category_t = std::vector<benchmark_t>;

}
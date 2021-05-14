#include <grapher/containers.hpp>
#include <grapher/core.hpp>
#include <grapher/parsing.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>

namespace grapher {

/// Raw entry (before proper extraction in an entry_t type)
using raw_entry_t = std::tuple<std::filesystem::path, nlohmann::json>;

/// Raw category
using raw_category_t = std::vector<entry_t>;

/// Reads benchmark size in filename
int benchmark_size_from_path(std::filesystem::path const &p) {
  std::size_t size;
  std::istringstream stem(p.stem());
  stem >> size;

  if (!stem) {
    std::cerr << "Invalid filename for " << p
              << ": must have <size>.json format." << std::ends;
    return -1;
  }

  return size;
}

/// Extracts a benchmark entry given a path
entry_t extract_entry(std::filesystem::path const &p) {
  entry_t res{
      .size = entry_t::nsize,
      .execute_compiler = 0,
      .frontend = 0,
      .source = 0,
      .instantiate_function = 0,
      .parse_class = 0,
      .instantiate_class = 0,
      .backend = 0,
      .opt_module = 0,
      .parse_template = 0,
      .opt_function = 0,
      .run_pass = 0,
      .per_module_passes = 0,
      .perform_pending_instantiations = 0,
      .run_loop_pass = 0,
      .code_gen_passes = 0,
      .code_gen_function = 0,
      .per_function_passes = 0,
  };

  // Reading JSON
  nlohmann::json js;
  {
    std::ifstream file(p);
    file >> js;
    if (!(file >> js)) {
      return res;
    }
  }

  // Reading filename for size
  {
    res.size = benchmark_size_from_path(p);

    if (res.size == entry_t::nsize) {
      std::cerr << "Invalid filename for " << p << std::ends;
      return res;
    }
  }

  for (auto e : js["traceEvents"]) {
    auto const &e_name = e["name"];

    if (e_name == "Total ExecuteCompiler") {
      res.execute_compiler = e["dur"];
    } else if (e_name == "Total Frontend") {
      res.frontend = e["dur"];
    } else if (e_name == "Total Source") {
      res.source = e["dur"];
    } else if (e_name == "Total InstantiateFunction") {
      res.instantiate_function = e["dur"];
    } else if (e_name == "Total ParseClass") {
      res.parse_class = e["dur"];
    } else if (e_name == "Total InstantiateClass") {
      res.instantiate_class = e["dur"];
    } else if (e_name == "Total Backend") {
      res.backend = e["dur"];
    } else if (e_name == "Total OptModule") {
      res.opt_module = e["dur"];
    } else if (e_name == "Total ParseTemplate") {
      res.parse_template = e["dur"];
    } else if (e_name == "Total OptFunction") {
      res.opt_function = e["dur"];
    } else if (e_name == "Total RunPass") {
      res.run_pass = e["dur"];
    } else if (e_name == "Total PerModulePasses") {
      res.per_module_passes = e["dur"];
    } else if (e_name == "Total PerformPendingInstantiations") {
      res.perform_pending_instantiations = e["dur"];
    } else if (e_name == "Total RunLoopPass") {
      res.run_loop_pass = e["dur"];
    } else if (e_name == "Total CodeGenPasses") {
      res.code_gen_passes = e["dur"];
    } else if (e_name == "Total CodeGen Function") {
      res.code_gen_function = e["dur"];
    } else if (e_name == "Total PerFunctionPasses") {
      res.per_function_passes = e["dur"];
    }
  }
  return res;
}

std::optional<benchmark_t>
extract_benchmark(std::filesystem::path const &benchmark_directory) {
  namespace fs = std::filesystem;

  std::vector<fs::path> entry_file_list =
      fs::is_directory(benchmark_directory)
          ? std::vector<fs::path>{begin(fs::directory_iterator(
                                      benchmark_directory)),
                                  end(fs::directory_iterator(
                                      benchmark_directory))}
          : std::vector<fs::path>{benchmark_directory};

  std::vector<entry_t> entries;

  for (auto const &p : entry_file_list) {
    // Check for valid file extension
    if (p.extension() != ".json") {
      std::cerr << "[WARNING] Invalid entry extension: " << p << std::ends;
      continue;
    }

    auto e = extract_entry(p);

    // Check for entry validity
    if (!e) {
      std::cerr << "[WARNING] Invalid entry format: " << p << std::ends;
      continue;
    }

    entries.push_back(extract_entry(p));
  }

  if (entries.empty()) {
    std::cerr << "Invalid benckmark " << benchmark_directory << ": No entry"
              << std::ends;
    return std::nullopt;
  }

  std::sort(entries.begin(), entries.end(),
            [](auto const &a, auto const &b) { return a.size < b.size; });

  auto const iterations = std::find_if(entries.begin(), entries.end(),
                                       [s = entries[0].size](entry_t const &e) {
                                         return e.size != s;
                                       }) -
                          entries.begin();

  auto const size = entries.size() - iterations;

  // Last check: entry size validity
  if (entries.size() != size * iterations) {
    std::cerr << "[ERROR] Invalid entry amount for benchnark "
              << benchmark_directory << std::ends;
    return std::nullopt;
  }

  benchmark_t res(benchmark_directory.filename(), size, iterations);

  for (std::size_t size_id; size_id < size; size_id++) {
    auto const sz = entries[size_id * iterations].size;
    for (std::size_t iteration; iteration < iterations; iteration++) {
      if (auto const e = entries[size_id * iterations + iteration];
          e.size != sz) {
        std::cerr << "[ERROR] Invalid data for benchmark "
                  << benchmark_directory << ": irregular benchmark sizes."
                  << std::ends;
        return std::nullopt;
      } else {
        res.store(e, size_id, iteration);
      }
    }
  }

  return res;
}

} // namespace grapher

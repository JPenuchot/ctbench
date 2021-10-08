#include <algorithm>
#include <filesystem>
#include <fstream>

#include <llvm/Support/raw_ostream.h>

#include "grapher/utils/cli.hpp"

namespace grapher {

grapher::benchmark_set_t
build_category(llvm::cl::list<std::string> const &benchmark_path_list) {
  namespace fs = std::filesystem;

  // Filling in category
  grapher::benchmark_set_t category;
  for (auto const &bench_path_str : benchmark_path_list) {
    fs::path bench_path(bench_path_str.data());

    if (!fs::is_directory(bench_path)) {
      llvm::errs() << "[WARNING] Not a directory: " << bench_path << '\n';
      continue;
    }

    grapher::benchmark_case_t bench;

    // Reading benchmark name from benchmark directory path
    bench.name = bench_path.filename();

    // Adding entries
    for (auto const &entry_dir : fs::directory_iterator(bench_path)) {
      grapher::benchmark_instance_t entry;

      // Entry directory name check and reading to entry size
      {
        std::istringstream iss(entry_dir.path().filename().stem());
        if (!(iss >> entry.size)) {
          llvm::errs() << "[WARNING] Entry directory name is not a size: "
                       << entry_dir.path() << '\n';
          continue;
        }
      }

      // Reading iterations
      for (auto const &iteration_path :
           fs::recursive_directory_iterator(entry_dir)) {
        // Basic property check
        if (!std::filesystem::is_regular_file(iteration_path)) {
          llvm::errs()
              << "[WARNING] Invalid iteration file (not a regular file): "
              << iteration_path.path() << '\n';
          continue;
        }

        // File content check and reading
        std::ifstream iteration_file(iteration_path.path());
        nlohmann::json j;
        if (!(iteration_file >> j)) {
          llvm::errs()
              << "[WARNING] Invalid iteration file (invalid JSON content): "
              << iteration_path.path() << '\n';
          continue;
        }

        entry.iterations.push_back(std::move(j));
      }
      bench.instances.push_back(entry);
    }

    // Sorting by entry size
    std::sort(
        bench.instances.begin(), bench.instances.end(),
        [](benchmark_instance_t const &a, benchmark_instance_t const &b) { return a.size < b.size; });

    category.push_back(std::move(bench));
  }
  return category;
}

} // namespace grapher

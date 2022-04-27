#include <algorithm>
#include <filesystem>
#include <fstream>

#include <llvm/Support/raw_ostream.h>

#include <fmt/core.h>

#include "grapher/utils/cli.hpp"
#include "grapher/utils/error.hpp"

namespace grapher {

grapher::benchmark_set_t
build_category(llvm::cl::list<std::string> const &benchmark_path_list) {
  namespace fs = std::filesystem;

  // Filling in benchmark set
  grapher::benchmark_set_t bset;
  for (auto const &bench_path_str : benchmark_path_list) {
    fs::path bench_path(bench_path_str.data());

    if (!fs::is_directory(bench_path)) {
      warn(fmt::format("Not a directory: {} (current path: {}).",
                       bench_path.string(), fs::current_path().string()));
      continue;
    }

    grapher::benchmark_case_t bench;

    // Reading benchmark name from benchmark directory path
    bench.name = bench_path.filename();

    // Adding entries
    for (auto const &entry_dir : fs::directory_iterator(bench_path)) {
      grapher::benchmark_iteration_t iteration;

      // Entry directory name check and reading to entry size
      {
        std::istringstream iss(entry_dir.path().filename().stem());
        if (!(iss >> iteration.size)) {
          warn(fmt::format(
              "Entry directory name is not a size: {} (current path: {}).",
              entry_dir.path().string(), fs::current_path().string()));
          continue;
        }
      }

      // Aggregating paths to repetition data files
      for (fs::directory_entry const &sample_path_entry :
           fs::recursive_directory_iterator(entry_dir)) {

        // Basic property check
        if (!std::filesystem::is_regular_file(sample_path_entry)) {
          warn(fmt::format("Invalid repetition file (not a regular file): {} "
                           "(current path: {}).",
                           sample_path_entry.path().string(),
                           fs::current_path().string()));
          continue;
        }

        // Adding path
        iteration.samples.push_back(sample_path_entry);
      }
      bench.iterations.push_back(iteration);
    }

    // Sorting by entry size
    std::sort(bench.iterations.begin(), bench.iterations.end(),
              [](benchmark_iteration_t const &a,
                 benchmark_iteration_t const &b) { return a.size < b.size; });

    bset.push_back(std::move(bench));
  }
  return bset;
}

} // namespace grapher

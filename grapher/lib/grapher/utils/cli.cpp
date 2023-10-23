#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <llvm/Support/raw_ostream.h>

#include <fmt/core.h>

#include <grapher/utils/cli.hpp>
#include <grapher/utils/error.hpp>

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
      grapher::benchmark_instance_t instance;

      // Entry directory name check and reading to entry size
      {
        std::istringstream iss(entry_dir.path().filename().stem());
        if (!(iss >> instance.size)) {
          warn(fmt::format(
              "Entry directory name is not a size: {} (current path: {}).",
              entry_dir.path().string(), fs::current_path().string()));
          continue;
        }
      }

      // Aggregating paths to repetition data files
      for (fs::directory_entry const &repetition_directory_entry :
           fs::recursive_directory_iterator(entry_dir)) {

        // Basic property check
        if (!std::filesystem::is_regular_file(repetition_directory_entry)) {
          warn(fmt::format("Invalid repetition file (not a regular file): {} "
                           "(current path: {}).",
                           repetition_directory_entry.path().string(),
                           fs::current_path().string()));
          continue;
        }

        // Adding path
        instance.repetitions.push_back(repetition_directory_entry);
      }
      bench.instances.push_back(instance);
    }

    // Sorting by entry size
    std::sort(bench.instances.begin(), bench.instances.end(),
              [](benchmark_instance_t const &a, benchmark_instance_t const &b) {
                return a.size < b.size;
              });

    bset.push_back(std::move(bench));
  }
  return bset;
}

} // namespace grapher

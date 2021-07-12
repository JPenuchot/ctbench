#include <grapher/cli-utils.hpp>

#include <filesystem>
#include <fstream>
#include <llvm/Support/raw_ostream.h>

namespace grapher {

grapher::category_t
build_category(llvm::cl::list<std::string> const &benchmark_path_list) {
  namespace fs = std::filesystem;

  // Filling in category
  grapher::category_t category;
  for (auto const &bench_path_str : benchmark_path_list) {
    llvm::outs() << "bench_path_str = " << bench_path_str << '\n';

    fs::path bench_path(bench_path_str.data());

    if (!fs::is_directory(bench_path)) {
      llvm::errs() << "[WARNING] Not a directory: " << bench_path << '\n';
      continue;
    }

    grapher::benchmark_t bench;

    // Reading benchmark name from benchmark directory path
    bench.name = bench_path.filename();

    // Adding entries
    for (auto const &entry_dir : fs::directory_iterator(bench_path)) {
      llvm::outs() << "entry_dir = " << entry_dir.path() << '\n';
      grapher::entry_t entry;

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
        llvm::outs() << "iteration_path = " << iteration_path.path() << '\n';

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

        entry.data.push_back(std::move(j));
      }
      bench.entries.push_back(entry);
    }
    category.push_back(std::move(bench));
  }
  return category;
}

} // namespace grapher

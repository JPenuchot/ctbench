#include <algorithm>
#include <filesystem>
#include <fstream>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

#include <grapher/core.hpp>
#include <grapher/graph.hpp>

llvm::cl::opt<llvm::StringRef>
    output_option(llvm::cl::Positional, llvm::cl::Required,
                  llvm::cl::value_desc("Output folder."));

llvm::cl::list<llvm::StringRef>
    benchmark_path_list(llvm::cl::Positional, llvm::cl::Required,
                        llvm::cl::value_desc("Benchmark folder list."));

int main(int argc, char const *argv[]) {
  namespace fs = std::filesystem;

  // Thanks LLVM
  llvm::cl::ParseCommandLineOptions(argc, argv);

  // Filling in category
  grapher::category_t category;
  for (auto const &elmt : benchmark_path_list) {
    fs::path bench_path(elmt.data());

    if (!fs::is_directory(bench_path)) {
      llvm::outs() << "[WARNING] Not a directory: " << bench_path << '\n';
      continue;
    }

    grapher::benchmark_t bench;

    // Reading benchmark name from benchmark directory path
    bench.name = bench_path.filename();

    // Adding entries
    for (auto const &entry_dir : fs::directory_iterator(bench_path)) {
      grapher::entry_t entry;

      // Entry directory name check and reading to entry size
      {
        std::istringstream iss(entry_dir.path());
        if (!iss >> entry.size) {
          llvm::outs() << "[WARNING] Entry directory name is not a size: "
                       << entry_dir.path() << '\n';
          continue;
        }
      }

      // Reading iterations
      for (auto const &iteration_path :
           fs::recursive_directory_iterator(entry_dir)) {

        // Basic property check
        if (!std::filesystem::is_regular_file(iteration_path)) {
          llvm::outs()
              << "[WARNING] Invalid iteration file (not a regular file): "
              << iteration_path.path() << '\n';
          continue;
        }

        // File content check and reading
        std::ifstream iteration_file(iteration_path.path());
        nlohmann::json j;
        if (!(iteration_file >> j)) {
          llvm::outs()
              << "[WARNING] Invalid iteration file (invalid JSON content): "
              << iteration_path.path() << '\n';
          continue;
        }

        entry.data.push_back(std::move(j));
      }
    }

    category.push_back(std::move(bench));
  }

  // Output
  // grapher::stacked_graph(category, {""_json}, argv[out_id]);

  return 0;
}

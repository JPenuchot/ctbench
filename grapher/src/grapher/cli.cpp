#include <filesystem>
#include <fstream>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

#include <grapher/cli.hpp>
#include <grapher/core.hpp>
#include <grapher/plotters/plotters.hpp>

namespace grapher::cli {

// llvm::cl docs: https://llvm.org/docs/CommandLine.html

//==============================================================================
// LLVM::CL DECLARATIONS

namespace lc = llvm::cl;

lc::opt<llvm::StringRef> command_opt(lc::Positional, lc::Required,
                                     lc::desc("<Command>"));

lc::opt<llvm::StringRef> filter_path_opt(lc::Positional, lc::Required,
                                         lc::desc("<JSON filter path>"));

lc::opt<llvm::StringRef> json_pointer_opt(lc::Positional, lc::Required,
                                          lc::desc("<JSON pointer>"));

lc::opt<llvm::StringRef> output_folder_opt(lc::Positional, lc::Required,
                                           lc::desc("<Output folder>"));

lc::list<llvm::StringRef> benchmark_path_list(lc::Positional, lc::OneOrMore,
                                              lc::desc("<Input folders>"));

lc::opt<llvm::StringRef> config_opt("c", "config",
                                    lc::desc("JSON configuration file."));

//==============================================================================
// DEFINITIONS

bool parse_cli_options(int argc, char const *argv[]) {
  return lc::ParseCommandLineOptions(argc, argv, "Grapher\n");
}

category_t build_category() {
  namespace fs = std::filesystem;

  // Filling in category
  category_t category;
  for (auto const &elmt : benchmark_path_list) {
    fs::path bench_path(elmt.data());

    if (!fs::is_directory(bench_path)) {
      llvm::outs() << "[WARNING] Not a directory: " << bench_path << '\n';
      continue;
    }

    benchmark_t bench;

    // Reading benchmark name from benchmark directory path
    bench.name = bench_path.filename();

    // Adding entries
    for (auto const &entry_dir : fs::directory_iterator(bench_path)) {
      entry_t entry;

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
  return category;
}

std::unique_ptr<plotter_i> select_plotter() {
  if (command_opt.empty()) {
    return std::make_unique<stacked_graph_t>();
  }

  if (command_opt.getValue() == "stacked") {
    return std::make_unique<stacked_graph_t>();
  }

  if (command_opt.getValue() == "compare") {
    return std::make_unique<comparative_graph_t>();
  }

  // TODO: Add warning
  return std::make_unique<comparative_graph_t>();
}

nlohmann::json get_config() {
  if (config_opt.empty()) {
    return {};
  }

  std::ifstream json_file(config_opt.getValue().data());

  nlohmann::json res;
  if (json_file >> res) {
    return res;
  }

  // TODO: Add warning (invalid config file)
  return {};
}

} // namespace grapher::cli

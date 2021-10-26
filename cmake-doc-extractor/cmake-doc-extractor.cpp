/// \file
/// Preprocessor for CMake public API documentation extraction.
/// In other words, it extracts comments that start with `#!`.

#include <algorithm>
#include <fstream>
#include <string>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

namespace cli {
namespace lc = llvm::cl;

lc::opt<std::string> input_path_opt("input", lc::desc("<input file>"),
                                    lc::Required);

lc::opt<std::string> output_path_opt("output", lc::desc("<output file>"),
                                     lc::Required);
} // namespace cli

int main(int argc, char const *argv[]) {
  llvm::cl::ParseCommandLineOptions(argc, argv);

  std::ifstream input_file(cli::input_path_opt.getValue());
  std::ofstream output_file(cli::output_path_opt.getValue());

  if (!input_file) {
    llvm::errs() << "[ERROR] Couldn't open input file "
                 << cli::input_path_opt.getValue() << ".\n";
    std::exit(1);
  }

  if (!output_file) {
    llvm::errs() << "[ERROR] Couldn't open output file "
                 << cli::output_path_opt.getValue() << ".\n";
    std::exit(1);
  }

  for (std::string line; std::getline(input_file, line);) {
    if (line.starts_with("#! ")) {
      output_file << line.substr(3) << '\n';
    } else if (line.starts_with("#!")) {
      output_file << line.substr(2) << '\n';
    }
  }

  return 0;
}

#include <grapher/core.hpp>
#include <grapher/graph.hpp>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/raw_ostream.h>

llvm::cl::opt<llvm::SmallString<64>>
    output_option(llvm::cl::Positional, llvm::cl::Required,
                  llvm::cl::value_desc("Output folder."));

llvm::cl::list<llvm::SmallString<64>>
    benchmark_path_list(llvm::cl::Positional, llvm::cl::Required,
                        llvm::cl::value_desc("Benchmark folder list."));

int main(int argc, char const *argv[]) {
  llvm::cl::ParseCommandLineOptions(argc, argv);
}

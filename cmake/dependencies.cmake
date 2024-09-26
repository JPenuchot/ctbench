# ctbench dependencies

# ctbench dependency management must be centralized here to avoid
# duplicate find_package calls and expose all dependencies in a single file.

find_package(nlohmann_json REQUIRED)
find_package(Boost REQUIRED)
find_package(sciplot REQUIRED)
find_package(LLVM REQUIRED CONFIG)
find_package(fmt REQUIRED)

if(${CTBENCH_ENABLE_TESTS})
  find_package(Catch2 REQUIRED)
endif()

if(${CTBENCH_ENABLE_DOCS})
  find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)
endif()

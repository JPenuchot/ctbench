# ctbench build options

set(CTBENCH_ENABLE_DOCS
    OFF
    CACHE BOOL "ctbench option: Enable documentation target")

set(CTBENCH_ENABLE_TESTS
    ON
    CACHE BOOL "ctbench option: Enable test targets")

set(CTBENCH_ENABLE_TRACY
    OFF
    CACHE BOOL "ctbench option: Enable Tracy profiler")

set(CTBENCH_ENABLE_CLANG_TIDY
    OFF
    CACHE BOOL "ctbench option: Enable clang tidy")

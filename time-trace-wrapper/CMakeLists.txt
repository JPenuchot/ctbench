add_executable(ctbench-ttw time-trace-wrapper.cpp)
target_include_directories(ctbench-ttw PUBLIC include)

set(CTBENCH_TIME_TRACE_WRAPPER_EXEC
    "${PROJECT_BINARY_DIR}/ctbench-ttw"
    PARENT_SCOPE)

if(BUILD_TESTING)
  add_executable(ctbench-ttw-test-exec tests/time-trace-wrapper.cpp)
  target_include_directories(ctbench-ttw-test-exec PUBLIC include)
  add_test(NAME ctbench-ttw-test COMMAND ctbench-ttw-test-exec)
endif(BUILD_TESTING)

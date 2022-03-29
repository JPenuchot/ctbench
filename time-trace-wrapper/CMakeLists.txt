add_executable(ctbench-time-trace-wrapper time-trace-wrapper.cpp)
target_include_directories(ctbench-time-trace-wrapper PRIVATE include)

install(TARGETS ctbench-time-trace-wrapper EXPORT ctbench-targets)

if(BUILD_TESTING)
  add_executable(ctbench-time-trace-wrapper-test-exec
                 tests/time-trace-wrapper.cpp)
  target_include_directories(ctbench-time-trace-wrapper-test-exec
                             PUBLIC include)
  add_test(NAME ctbench-time-trace-wrapper-test
           COMMAND ctbench-time-trace-wrapper-test-exec)
endif(BUILD_TESTING)

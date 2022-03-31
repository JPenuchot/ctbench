add_executable(ctbench-ttw ttw.cpp)
target_include_directories(ctbench-ttw PRIVATE include)

if(BUILD_TESTING)
  add_executable(ctbench-ttw-test-exec tests/ttw.cpp)
  target_include_directories(ctbench-ttw-test-exec PRIVATE include)
  add_test(NAME ctbench-ttw-test COMMAND ctbench-ttw-test-exec)
endif(BUILD_TESTING)

# Grapher library target declaration
include(cmake/grapher-target.cmake)

# Adding executables

add_executable(ctbench-grapher-plot grapher-plot.cpp)
target_link_libraries(ctbench-grapher-plot PRIVATE grapher)

add_executable(ctbench-grapher-utils grapher-utils.cpp)
target_link_libraries(ctbench-grapher-utils PRIVATE grapher)

# Profiler integration
if(CTBENCH_ENABLE_TRACY)
  include(cmake/tracy.cmake)
endif()

# Test targets
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

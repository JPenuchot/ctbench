file(GLOB_RECURSE SOURCES grapher/*.cpp)

add_executable(grapher-tests ${SOURCES})
target_link_libraries(grapher-tests PRIVATE Catch2::Catch2WithMain grapher)

add_custom_target(test-grapher grapher-tests)
add_dependencies(test-all test-grapher)

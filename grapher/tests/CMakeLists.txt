file(GLOB_RECURSE SOURCES grapher/*.cpp)

add_executable(grapher-tests ${SOURCES} main.cpp)
target_link_libraries(grapher-tests PRIVATE Catch2::Catch2 grapher)

add_custom_target(test-grapher grapher-tests)
add_dependencies(test-all test-grapher)

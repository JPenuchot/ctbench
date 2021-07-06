file(GLOB_RECURSE GRAPHER_UTILS_SOURCES src/*.cpp)

add_executable(grapher-utils ${GRAPHER_UTILS_SOURCES})

target_link_libraries(grapher-utils PRIVATE grapher)
target_include_directories(grapher-utils PRIVATE include)

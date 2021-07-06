file(GLOB_RECURSE GRAPHER_PLOT_SOURCES src/*.cpp)

add_executable(grapher-plot ${GRAPHER_PLOT_SOURCES})

target_link_libraries(grapher-plot PRIVATE grapher)
target_include_directories(grapher-plot PRIVATE include)

cmake_minimum_required(VERSION 3.18)
project(grapher)

file(GLOB_RECURSE GRAPHER_SOURCES "src/*.cpp")
add_executable(grapher ${GRAPHER_SOURCES} ${IMPLOT_SOURCES})

target_include_directories(grapher PRIVATE
  "include/"
  ${CONAN_INCLUDE_DIRS_NLOHMANN_JSON})

find_package(sciplot REQUIRED)
target_link_libraries(grapher sciplot::sciplot)

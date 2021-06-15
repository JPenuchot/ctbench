cmake_minimum_required(VERSION 3.20)
project(grapher)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE GRAPHER_SOURCES "src/*.cpp")

find_package(sciplot REQUIRED)
link_libraries(sciplot::sciplot)
include_directories("include/" ${CONAN_INCLUDE_DIRS_NLOHMANN_JSON})

# Declaring executable targets

add_executable(grapher-compare
  grapher-compare.cpp ${GRAPHER_SOURCES})

add_executable(grapher-stacked-curves
  grapher-stacked-curves.cpp ${GRAPHER_SOURCES})

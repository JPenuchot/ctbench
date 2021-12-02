# Doxygen target declaration
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)

# Documentation options
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_EXCLUDE_PATTERNS */build/* */tests/*)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/readme.md)
set(DOXYGEN_EXAMPLE_PATH ${CMAKE_SOURCE_DIR})
set(DOXYGENT_DOT_TRANSPARENT YES)

# ==============================================================================
# Doxygen Awesome CSS

include(FetchContent)

FetchContent_Declare(
  dac_content
  GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
  GIT_TAG v1.6.1
  GIT_SHALLOW)

FetchContent_GetProperties(dac_content)
if(NOT dac_content_POPULATED)
  FetchContent_Populate(dac_content)
endif()

set(DOXYGEN_HTML_EXTRA_STYLESHEET ${dac_content_SOURCE_DIR}/doxygen-awesome.css)

doxygen_add_docs(docs ALL)

# Documentation generation for the CMake API

add_custom_target(
  extract-api-doc
  cmake-doc-extractor
    --input ${CMAKE_SOURCE_DIR}/benchmarking.cmake
    --output ${CMAKE_SOURCE_DIR}/generated-docs/benchmarking.md
  DEPENDS ${CMAKE_SOURCE_DIR}/benchmarking.cmake)

add_dependencies(docs extract-api-doc)

include(FetchContent)

# ==============================================================================
# Doxygen Awesome CSS

FetchContent_Declare(dac_content
  GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
  GIT_TAG v1.5.0
  GIT_SHALLOW)

FetchContent_GetProperties(dac_content)
if(NOT dac_content_POPULATED)
  FetchContent_Populate(dac_content)
endif()

# Doxygen target declaration
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)

set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_EXCLUDE_PATTERNS */build/* */tests/*)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${CMAKE_SOURCE_DIR}/readme.md)
set(DOXYGEN_EXAMPLE_PATH ${CMAKE_SOURCE_DIR})

# Styling

set(DOXYGEN_HTML_COLORSTYLE_HUE 209)
set(DOXYGEN_HTML_COLORSTYLE_SAT 255)
set(DOXYGEN_HTML_COLORSTYLE_GAMMA 113)

set(DOXYGEN_HTML_EXTRA_STYLESHEET ${dac_content_SOURCE_DIR}/doxygen-awesome.css)

doxygen_add_docs(docs ALL)

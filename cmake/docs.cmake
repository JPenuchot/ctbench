# Doxygen target declaration

find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)

set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_EXCLUDE_PATTERNS */build/*)

# Styling

set(DOXYGEN_HTML_COLORSTYLE_HUE 209)
set(DOXYGEN_HTML_COLORSTYLE_SAT 255)
set(DOXYGEN_HTML_COLORSTYLE_GAMMA 113)

# ${dac_content_SOURCE_DIR}
set(DOXYGEN_HTML_EXTRA_STYLESHEET ${dac_content_SOURCE_DIR}/doxygen-awesome.css)

doxygen_add_docs(docs ALL)

include(FetchContent)

# ==============================================================================
# nlohmann::json

set(JSON_BuildTests OFF)
set(JSON_Install OFF)
set(JSON_MultipleHeaders ON)
set(JSON_ImplicitConversions ON)
set(JSON_Diagnostics ON)
set(JSON_CI OFF)

FetchContent_Declare(json_content
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.9.1)

FetchContent_MakeAvailable(json_content)

# ==============================================================================
# Sciplot

FetchContent_Declare(sciplot_content
  GIT_REPOSITORY https://github.com/sciplot/sciplot.git
  GIT_TAG master)

FetchContent_GetProperties(sciplot_content)
if(NOT sciplot_content_POPULATED)
  FetchContent_Populate(sciplot_content)
endif()

# ==============================================================================
# TTS

if(CTBENCH_ENABLE_TESTING)
  FetchContent_Declare(tts_content
    GIT_REPOSITORY https://github.com/jfalcou/tts.git
    GIT_TAG 1.0)

  FetchContent_GetProperties(tts_content)
  if(NOT tts_content_POPULATED)
    FetchContent_Populate(tts_content)
  endif()
endif()

# ==============================================================================
# Doxygen Awesome CSS

FetchContent_Declare(dac_content
  GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
  GIT_TAG v1.5.0)

FetchContent_GetProperties(dac_content)
if(NOT dac_content_POPULATED)
  FetchContent_Populate(dac_content)
endif()


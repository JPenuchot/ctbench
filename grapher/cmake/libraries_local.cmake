# ==============================================================================
# nlohmann::json

find_package(nlohmann_json 3.9.1 REQUIRED)
target_link_libraries(grapher PUBLIC nlohmann_json::nlohmann_json)

# ==============================================================================
# Sciplot

find_package(sciplot REQUIRED)
target_link_libraries(grapher PUBLIC sciplot::sciplot)

# ==============================================================================
# TTS

# https://jfalcou.github.io/tts/index.html#buildandsetup

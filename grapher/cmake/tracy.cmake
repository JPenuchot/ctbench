# Optional: Tracy integration for profiling
# See: https://github.com/wolfpld/tracy

# Fetch Tracy
FetchContent_Declare(
  tracy
  GIT_REPOSITORY https://github.com/wolfpld/tracy.git
  GIT_TAG master
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE)
FetchContent_MakeAvailable(tracy)

# Link and enable Tracy
target_link_libraries(grapher PUBLIC Tracy::TracyClient)
target_compile_definitions(grapher PUBLIC TRACY_ENABLE=1)

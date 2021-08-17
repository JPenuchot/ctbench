add_custom_target(ctbench_tests)

function(ctbench_add_test file)
  # Target name generation
  file(RELATIVE_PATH target_name ${CMAKE_CURRENT_SOURCE_DIR} ${file})
  string(REPLACE ".cpp" "" target_name ${target_name})
  string(REPLACE "/" "." target_name ${target_name})

  # Compile target
  add_executable(${target_name}_executable ${file})
  target_link_libraries(${target_name}_executable PRIVATE grapher)
  target_include_directories(${target_name}_executable
                             PRIVATE ${tts_content_SOURCE_DIR}/include)

  # Exec target
  add_custom_target(${target_name} ${target_name}_executable)
  add_dependencies(ctbench_tests ${target_name})
endfunction()

file(GLOB_RECURSE test_files "*.cpp")
foreach(test_file ${test_files})
  ctbench_add_test(${test_file})
endforeach()

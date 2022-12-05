add_executable(ctbench-ttw ttw.cpp)
target_link_libraries(ctbench-ttw PRIVATE ctbench-compile-opts)
target_include_directories(ctbench-ttw PRIVATE include)

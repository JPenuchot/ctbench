include(CMakePackageConfigHelpers)

set(ConfigPackageLocation lib/cmake/${PROJECT_NAME})

install(TARGETS ctbench-grapher-plot ctbench-grapher-utils ctbench-ttw
        EXPORT ctbench-targets)

export(
  EXPORT ctbench-targets
  NAMESPACE ctbench::
  FILE ctbench-targets.cmake)

install(EXPORT ctbench-targets DESTINATION ${ConfigPackageLocation})
install(FILES cmake/ctbench-api.cmake DESTINATION ${ConfigPackageLocation})

# CMake package config files

configure_package_config_file(cmake/ctbench-config.cmake.in ctbench-config.cmake
                              INSTALL_DESTINATION ${ConfigPackageLocation})

write_basic_package_version_file(
  ctbench-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_BINARY_DIR}/ctbench-config.cmake
              ${CMAKE_BINARY_DIR}/ctbench-config-version.cmake
        DESTINATION ${ConfigPackageLocation})

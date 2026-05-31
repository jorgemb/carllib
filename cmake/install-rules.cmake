if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/carllib-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package carllib)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT carllib_Development
)

install(
    TARGETS carllib_carllib
    EXPORT carllibTargets
    RUNTIME #
    COMPONENT carllib_Runtime
    LIBRARY #
    COMPONENT carllib_Runtime
    NAMELINK_COMPONENT carllib_Development
    ARCHIVE #
    COMPONENT carllib_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    carllib_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE carllib_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(carllib_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${carllib_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT carllib_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${carllib_INSTALL_CMAKEDIR}"
    COMPONENT carllib_Development
)

install(
    EXPORT carllibTargets
    NAMESPACE carllib::
    DESTINATION "${carllib_INSTALL_CMAKEDIR}"
    COMPONENT carllib_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()

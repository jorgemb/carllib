cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    carllib
    VERSION 0.1.0
    DESCRIPTION "Basic library for Cellular Automata and Reinforcement learning with drawing capabilities."
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----
find_package(SFML 3 REQUIRED COMPONENTS Graphics)
find_package(spdlog REQUIRED)
find_package(tomlplusplus REQUIRED)
#find_package(OpenGL REQUIRED COMPONENTS GLX OpenGL)

add_library(
    carllib_carllib
    source/carllib.cpp
        source/window.cpp
        include/carllib/graphics/window.hpp
        source/cell_grid.cpp
        include/carllib/graphics/cell_grid.hpp
        source/ca/linear_toroidal.cpp
        include/carllib/ca/linear_toroidal.hpp
        include/carllib/util/math.hpp
)
add_library(carllib::carllib ALIAS carllib_carllib)

target_link_libraries(
        carllib_carllib PUBLIC
        SFML::Graphics
        spdlog::spdlog
        tomlplusplus::tomlplusplus
#        OpenGL
)



include(GenerateExportHeader)
generate_export_header(
    carllib_carllib
    BASE_NAME carllib
    EXPORT_FILE_NAME export/carllib/carllib_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(carllib_carllib PUBLIC CARLLIB_STATIC_DEFINE)
endif()

set_target_properties(
    carllib_carllib PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME carllib
    OUTPUT_NAME carllib
)

target_include_directories(
    carllib_carllib ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    carllib_carllib SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(carllib_carllib PUBLIC cxx_std_23)

# ---- Declare executable ----

find_package(fmt REQUIRED)

add_executable(carllib_exe source/main.cpp)
add_executable(carllib_exe::exe ALIAS carllib_exe)

set_property(TARGET carllib_exe PROPERTY OUTPUT_NAME carllib_main)

target_compile_features(carllib_exe PRIVATE cxx_std_23)

target_link_libraries(carllib_exe PRIVATE carllib::carllib fmt::fmt)


# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT carllib_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of carllib"
  )
endif()

include(cmake/dev-mode.cmake)

# ---- Run target ----

# Convenience target: builds carllib_exe and runs it from the source root so
# that relative config paths (e.g. init.toml) resolve correctly.
add_custom_target(
    run
    COMMAND carllib_exe
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    USES_TERMINAL
    VERBATIM
)

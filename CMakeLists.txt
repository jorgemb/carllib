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

add_library(
    carllib_carllib
    source/carllib.cpp
)
add_library(carllib::carllib ALIAS carllib_carllib)

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

target_compile_features(carllib_carllib PUBLIC cxx_std_20)

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

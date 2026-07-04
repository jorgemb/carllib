cmake_minimum_required(VERSION 3.14)

project(carllibTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if (PROJECT_IS_TOP_LEVEL)
    find_package(carllib REQUIRED)
    enable_testing()
endif ()

find_package(Catch2 3 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(carllib_test
        source/carllib_test.cpp
        source/math_test.cpp
        source/base_1d_test.cpp
        source/wolfram_test.cpp
        source/initial_condition_test.cpp
)
target_link_libraries(
        carllib_test PRIVATE
        carllib::carllib
        Catch2::Catch2WithMain
)
target_compile_features(carllib_test PRIVATE cxx_std_26)

# Register each Catch2 TEST_CASE as a separate CTest test.
catch_discover_tests(
        carllib_test
        ADD_TAGS_AS_LABELS
)

# ---- End-of-file commands ----

add_folders(Test)

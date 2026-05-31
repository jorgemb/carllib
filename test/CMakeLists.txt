cmake_minimum_required(VERSION 3.14)

project(carllibTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(carllib REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(carllib_test source/carllib_test.cpp)
target_link_libraries(carllib_test PRIVATE carllib::carllib)
target_compile_features(carllib_test PRIVATE cxx_std_20)

add_test(NAME carllib_test COMMAND carllib_test)

# ---- End-of-file commands ----

add_folders(Test)

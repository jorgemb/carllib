cmake_minimum_required(VERSION 3.14)

# ---- Declare executable ----

find_package(fmt REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

add_executable(wolfram main.cpp)
add_executable(wolfram::exe ALIAS wolfram)

set_property(TARGET wolfram PROPERTY OUTPUT_NAME wolfram)

target_compile_features(wolfram PRIVATE cxx_std_23)

target_link_libraries(wolfram PRIVATE carllib::carllib
        fmt::fmt
        CLI11::CLI11
)

cmake_minimum_required(VERSION 3.20)
project(nhom2_edge_dp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

# GoogleTest / Catch2 - prefer vendored single-header catch2.
find_package(Catch2 REQUIRED)

add_library(baseline_dp OBJECT src/baseline_dp.cpp)
add_library(dnc_dp      OBJECT src/dnc_dp.cpp)
add_library(cht         OBJECT src/cht.cpp src/cht_li_chao.cpp)
add_library(knuth       OBJECT src/knuth.cpp)

add_executable(test_baseline tests/test_baseline.cpp $<TARGET_OBJECTS:baseline_dp>)
add_executable(test_dnc      tests/test_dnc.cpp      $<TARGET_OBJECTS:baseline_dp> $<TARGET_OBJECTS:dnc_dp>)
add_executable(test_knuth    tests/test_knuth.cpp    $<TARGET_OBJECTS:knuth>)

target_link_libraries(test_baseline PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_dnc      PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_knuth    PRIVATE Catch2::Catch2WithMain)

enable_testing()
add_test(NAME baseline COMMAND test_baseline)
add_test(NAME dnc      COMMAND test_dnc)
add_test(NAME knuth    COMMAND test_knuth)

option(ENABLE_COVERAGE "Enable coverage" OFF)
if(ENABLE_COVERAGE)
  add_compile_options(--coverage -O0)
  add_link_options(--coverage)
endif()

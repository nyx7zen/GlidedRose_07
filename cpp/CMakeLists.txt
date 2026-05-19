cmake_minimum_required(VERSION 3.14)
project(GildedRose)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    TLS_VERIFY OFF
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Library
add_library(gilded_rose src/GildedRose.cpp)
target_include_directories(gilded_rose PUBLIC include)

# Tests
enable_testing()
add_executable(gilded_rose_test test/GildedRoseTest.cpp)
target_link_libraries(gilded_rose_test gilded_rose GTest::gtest_main)

add_executable(golden_master_test test/GoldenMasterTest.cpp)
target_link_libraries(golden_master_test gilded_rose GTest::gtest_main)
target_compile_definitions(golden_master_test PRIVATE
    GOLDEN_MASTER_EXPECTED_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test/golden_master"
)

include(GoogleTest)
gtest_discover_tests(gilded_rose_test)
gtest_discover_tests(golden_master_test)

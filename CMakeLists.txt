cmake_minimum_required(VERSION 3.13)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)
enable_testing()

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
)

add_executable(NetSim ${SOURCE_FILES} main.cpp)

# --- TESTS ---

add_executable(NetSimTests
        tests/main_gtest.cpp
        tests/tests.cpp
)

target_link_libraries(NetSimTests
        gtest
        gtest_main
)

add_test(NAME NetSimTests COMMAND NetSimTests)


cmake_minimum_required(VERSION 3.13)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(INSTALL_GTEST OFF)
FetchContent_MakeAvailable(googletest)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
)

add_library(NetSimLib ${SOURCE_FILES})
target_include_directories(NetSimLib PUBLIC include)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    add_executable(NetSim main.cpp)
    target_link_libraries(NetSim NetSimLib)
endif()

enable_testing()

set(TEST_SOURCES
        tests/main_gtest.cpp
        tests/test_Factory.cpp
        tests/test_factory_io.cpp
        tests/test_nodes.cpp
        tests/test_package.cpp
        tests/test_reports.cpp
        tests/test_simulate.cpp
        tests/test_storage_types.cpp
        tests/tests.cpp
)

add_executable(NetSimTests ${TEST_SOURCES})

target_link_libraries(NetSimTests
        NetSimLib
        gtest
        gmock
        gtest_main
)

target_include_directories(NetSimTests PRIVATE mocks)

add_test(NAME NetSimTests COMMAND NetSimTests)
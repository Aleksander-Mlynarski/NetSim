make_minimum_required(VERSION 3.13)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)


include_directories(${CMAKE_SOURCE_DIR}/include)


set(SOURCE_FILES
        src/package.cpp
        src/test.cpp
        src/storage_types.cpp
)

add_executable(NetSim ${SOURCE_FILES} main.cpp)
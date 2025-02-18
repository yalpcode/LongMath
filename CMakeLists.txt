cmake_minimum_required(VERSION 3.19)
project(LongMath)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(benchmark REQUIRED)

add_executable(main main.cpp)

include_directories(LongNum)
add_subdirectory(LongNum)

target_link_libraries(main LongNum benchmark::benchmark)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    gflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    GIT_TAG v2.2.2
)

FetchContent_MakeAvailable(gflags)

enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(pitest PiTest.cpp)
target_link_libraries(pitest benchmark::benchmark)
target_link_libraries(pitest LongNum)

target_link_libraries(pitest gtest gtest_main)
target_link_libraries(pitest gflags::gflags)

add_executable(mathtest MathTest.cpp)
target_link_libraries(mathtest LongNum)

target_link_libraries(mathtest gtest gtest_main)
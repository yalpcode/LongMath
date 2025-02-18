cmake_minimum_required(VERSION 3.19)
project(LongMath)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.1 
)
FetchContent_MakeAvailable(benchmark)

FetchContent_Declare(
    gflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    GIT_TAG v2.2.2
)

FetchContent_MakeAvailable(gflags)

add_executable(main main.cpp)

include_directories(LongNum)
add_subdirectory(LongNum)

target_link_libraries(main LongNum benchmark::benchmark)

enable_testing()

add_executable(pitest PiTest.cpp)
target_link_libraries(pitest benchmark::benchmark)
target_link_libraries(pitest LongNum)
target_link_libraries(pitest gtest gtest_main)
target_link_libraries(pitest gflags::gflags)

add_executable(mathtest MathTest.cpp)
target_link_libraries(mathtest LongNum)
target_link_libraries(mathtest gtest gtest_main)
cmake_minimum_required(VERSION 3.19)
project(LongMath)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp)

include_directories(LongNum)
add_subdirectory(LongNum)

target_link_libraries(main LongNum)
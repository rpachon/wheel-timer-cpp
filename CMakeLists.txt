cmake_minimum_required(VERSION 3.6)
project(wheel_timer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(test)

set(SOURCE_FILES main.cpp)
add_executable(wheel_timer ${SOURCE_FILES})

target_link_libraries(wheel_timer src)
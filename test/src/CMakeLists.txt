add_executable(wheel-timerTest WheelTimerTest.cpp util/MockTimeOutable.h)

target_link_libraries(wheel-timerTest gtest gmock gtest_main)
target_link_libraries(wheel-timerTest src)

set(SOURCE_FILES benchmark/BenchmarkTimeout.cpp benchmark/BenchmarkTimeout.h benchmark/JobTimeoutable.cpp benchmark/JobTimeoutable.h)
add_executable(wheel_timer_bench ${SOURCE_FILES})
#
target_link_libraries(wheel_timer_bench src pthread)
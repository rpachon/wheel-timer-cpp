add_executable(wheel-timerTest WheelTest.cpp WheelTimerTest.cpp util/MockTimeOutable.h)

target_link_libraries(wheel-timerTest gtest gmock gtest_main)
target_link_libraries(wheel-timerTest src)
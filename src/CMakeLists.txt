project(wheel-timer)


set(SOURCE_FILES Wheel.h WheelTimer.h WheelTimer.cpp util/TimeOutable.h util/TimeoutItem.cpp util/TimeoutItem.h)
add_library(src ${SOURCE_FILES})
#ifndef WHEEL_TIMER_MOCKTIMEOUTABLE_H
#define WHEEL_TIMER_MOCKTIMEOUTABLE_H


#include <gmock/gmock-generated-function-mockers.h>
#include "../../../src/util/TimeOutable.h"

class MockTimeOutable : public TimeOutable {

public:
    MOCK_CONST_METHOD0(timeout, void());
    MOCK_CONST_METHOD0(isRunning, bool());
    MockTimeOutable() {};
};


#endif //WHEEL_TIMER_MOCKTIMEOUTABLE_H

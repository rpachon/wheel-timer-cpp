//
// Created by stan on 19/12/16.
//

#ifndef WHEEL_TIMER_MOCKTIMEOUTABLE_H
#define WHEEL_TIMER_MOCKTIMEOUTABLE_H


#include <gmock/gmock-generated-function-mockers.h>
#include "../../../src/util/TimeOutable.h"

class MockTimeOutable : public TimeOutable {

public:
    MOCK_METHOD0(timeout, void());
    MockTimeOutable() {};
};


#endif //WHEEL_TIMER_MOCKTIMEOUTABLE_H

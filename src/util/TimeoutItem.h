//
// Created by stan on 19/12/16.
//

#ifndef WHEEL_TIMER_TIMEOUTITEM_H
#define WHEEL_TIMER_TIMEOUTITEM_H


#include <chrono>
#include "TimeOutable.h"

using namespace std;

class TimeoutItem {

public:
    TimeOutable& timeOutable;
    TimeoutItem(TimeOutable& timeOutable, chrono::milliseconds timeout);
    chrono::milliseconds getTimeout();
    void updateTimeout(chrono::milliseconds timeout);

private:
    chrono::milliseconds timeout;

};


#endif //WHEEL_TIMER_TIMEOUTITEM_H

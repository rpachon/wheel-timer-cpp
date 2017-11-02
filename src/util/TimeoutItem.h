#ifndef WHEEL_TIMER_TIMEOUTITEM_H
#define WHEEL_TIMER_TIMEOUTITEM_H


#include <chrono>
#include "TimeOutable.h"


class TimeoutItem {

    typedef std::chrono::milliseconds Milliseconds;

public:
    const TimeOutable& timeOutable;
    TimeoutItem(const TimeOutable& timeOutable, const Milliseconds timeout);
    const Milliseconds& getTimeout() const noexcept;
    void updateTimeout(const Milliseconds &timeout) noexcept;

private:
    Milliseconds timeout;

};


#endif //WHEEL_TIMER_TIMEOUTITEM_H

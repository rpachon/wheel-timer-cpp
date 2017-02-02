#ifndef WHEEL_TIMER_WHEELTIMER_H
#define WHEEL_TIMER_WHEELTIMER_H

#include <chrono>
#include <mutex>
#include <thread>
#include "gtest/gtest_prod.h"
#include "Wheel.h"
#include "util/TimeoutItem.h"

using namespace std;

class WheelTimer {
    FRIEND_TEST(WheelTimer, should_timeout_all_items_with_tick_duration_at_one_ms);
    FRIEND_TEST(WheelTimer, should_timeout_all_items_with_tick_duration_at_thirty_ms);

    typedef chrono::milliseconds Milliseconds;

public:
    WheelTimer(const Milliseconds tickDuration, const Milliseconds maxTimeout);
    void add(TimeoutItem& timeoutItem);
    void start();
    void stop();;

private:
    static const unsigned int FIRST_WHEEL_SIZE;
    static const unsigned int OTHER_WHEEL_SIZE;

    mutex lock;
    thread *timer;

    volatile bool isStart;

    void run();

    vector<Wheel<TimeoutItem>> wheels;
    const Milliseconds tickDuration;

    unsigned int computeWheelNumber(const Milliseconds maxTimeout) const;
    void createWheels(const unsigned int wheelNumber);

    void cascade(vector<TimeoutItem> *timeoutItems);

    void tick();

    void computeAndAdd(TimeoutItem &timeoutItem);
};


#endif //WHEEL_TIMER_WHEELTIMER_H

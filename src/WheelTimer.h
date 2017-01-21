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
    FRIEND_TEST(WheelTimer, should_create_wheels_function_of_max_timeout);
    FRIEND_TEST(WheelTimer, should_add_an_item_in_the_first_wheel_when_timeout_between_1_and_256_multiply_tick_duration);
    FRIEND_TEST(WheelTimer, should_add_an_item_in_the_second_wheel_when_timeout_between_257_and_16384_multiply_tick_duration);
    FRIEND_TEST(WheelTimer, should_add_an_item_in_the_third_wheel_timeout_between_16641_and_1065216_multiply_tick_duration);
    FRIEND_TEST(WheelTimer, should_add_an_item_in_first_wheel_when_wheel_has_already_tick);
    FRIEND_TEST(WheelTimer, should_add_an_item_in_second_wheel_when_wheels_has_already_tick);
    FRIEND_TEST(WheelTimer, should_add_an_item_in_third_wheel_when_wheels_has_already_tick);
    FRIEND_TEST(WheelTimer, should_compute_remaining_time_when_adding_in_wheels);
    FRIEND_TEST(WheelTimer, should_cascade_bucket_if_not_in_the_first_wheel);
    FRIEND_TEST(WheelTimer, should_timeout_when_timeout_is_between_two_ticks);

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

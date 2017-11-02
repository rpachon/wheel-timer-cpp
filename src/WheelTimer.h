#ifndef WHEEL_TIMER_WHEELTIMER_H
#define WHEEL_TIMER_WHEELTIMER_H

#include <chrono>
#include <mutex>
#include <thread>
#include "gtest/gtest_prod.h"
#include "Wheel.h"
#include "util/TimeoutItem.h"

class WheelTimer {
    FRIEND_TEST(WheelTimer, should_timeout_all_items_with_tick_duration_at_one_ms);
    FRIEND_TEST(WheelTimer, should_timeout_all_items_with_tick_duration_at_thirty_ms);

    typedef std::chrono::milliseconds Milliseconds;

public:
    WheelTimer(const Milliseconds tickDuration, const Milliseconds maxTimeout);
    void add(TimeoutItem& timeoutItem) noexcept;
    void start() noexcept;
    void stop() noexcept ;

private:
    static const unsigned int FIRST_WHEEL_SIZE;
    static const unsigned int OTHER_WHEEL_SIZE;

    std::mutex lock;
    std::unique_ptr<std::thread> timer;

    volatile bool isStart;

    void run() noexcept;

    std::vector<Wheel<TimeoutItem>> wheels;
    const Milliseconds tickDuration;

    unsigned int computeWheelNumber(const Milliseconds &maxTimeout) const noexcept;
    void createWheels(const unsigned int wheelNumber) noexcept;

    void cascade(std::unique_ptr<std::vector<TimeoutItem>> timeoutItems) noexcept;

    void tick() noexcept;

    void computeAndAdd(TimeoutItem &timeoutItem) noexcept;
};


#endif //WHEEL_TIMER_WHEELTIMER_H

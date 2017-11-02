#include <cmath>
#include "WheelTimer.h"


const unsigned int WheelTimer::FIRST_WHEEL_SIZE = 256;
const unsigned int WheelTimer::OTHER_WHEEL_SIZE = 64;

WheelTimer::WheelTimer(const Milliseconds tickDuration, const Milliseconds maxTimeout) : tickDuration{tickDuration} {
    auto wheelNumber = computeWheelNumber(maxTimeout);
    createWheels(wheelNumber);
}

void WheelTimer::createWheels(const unsigned int wheelNumber) noexcept {
    Wheel<TimeoutItem> wheel{FIRST_WHEEL_SIZE};
    wheels.push_back(wheel);

    for (int i = 1; i < wheelNumber; ++i) {
        Wheel<TimeoutItem> wheel{OTHER_WHEEL_SIZE};
        wheels.push_back(wheel);
    }
}

unsigned int WheelTimer::computeWheelNumber(const Milliseconds &maxTimeout) const noexcept {
    unsigned int wheelNumber = 0;
    auto timePerWheel = FIRST_WHEEL_SIZE * tickDuration.count();

    auto maxTimeOutInMillis = maxTimeout.count();
    while(maxTimeOutInMillis > 0) {
        maxTimeOutInMillis -= timePerWheel * pow((double)OTHER_WHEEL_SIZE, (double)wheelNumber++);
    }
    return wheelNumber;
}

void WheelTimer::add(TimeoutItem& timeoutItem) noexcept {
    lock.lock();
    computeAndAdd(timeoutItem);
    lock.unlock();
}

void WheelTimer::computeAndAdd(TimeoutItem &timeoutItem) noexcept {
    auto timeoutValueInMillis = timeoutItem.getTimeout().count();
    auto currentWheelTime = FIRST_WHEEL_SIZE * tickDuration.count();
    auto bucketDuration = tickDuration.count();
    timeoutValueInMillis -= bucketDuration;

    for (auto it = wheels.begin(); it != wheels.end(); ++it) {
        if (timeoutValueInMillis < currentWheelTime) {
            auto bucket = (unsigned int) (timeoutValueInMillis / bucketDuration);
            timeoutItem.updateTimeout(Milliseconds(timeoutValueInMillis - bucket * bucketDuration));
            (*it).add(timeoutItem, bucket + 1);
            break;
        }
        timeoutValueInMillis -= ((*it).remainingTick() - 1) * bucketDuration;
        bucketDuration = currentWheelTime;
        currentWheelTime *= OTHER_WHEEL_SIZE;
    }
}

void WheelTimer::cascade(std::unique_ptr<std::vector<TimeoutItem>> timeoutItems) noexcept {
    for (auto it = timeoutItems->begin(); it != timeoutItems->end(); ++it) {
        if ((*it).timeOutable.isRunning()) {
            if ((*it).getTimeout().count() < tickDuration.count()) {
                (*it).timeOutable.timeout();
            } else {
                computeAndAdd(*it);
            }
        }
    }
}

void WheelTimer::tick() noexcept {
    lock.lock();
    for (int i = 0; i < wheels.size(); ++i) {
        cascade(wheels[i].nextBucket());
        if (!wheels[i].hasCascade()) {
            break;
        }
    }
    lock.unlock();
}


void WheelTimer::start() noexcept {
    isStart = true;
    timer = std::make_unique<std::thread>(&WheelTimer::run, this);
}

void WheelTimer::stop() noexcept {
    isStart = false;
    timer->join();
}


void WheelTimer::run() noexcept {
    const long tickDurationNano = std::chrono::duration_cast<std::chrono::nanoseconds>(tickDuration).count();
    long delta = std::chrono::high_resolution_clock::now().time_since_epoch().count() + tickDurationNano;

    while (isStart) {
        long now = std::chrono::high_resolution_clock::now().time_since_epoch().count();

        if (now >= delta) {
            delta += tickDurationNano;
            tick();
        }
    }
}


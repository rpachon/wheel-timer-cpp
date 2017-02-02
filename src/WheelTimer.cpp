#include <cmath>
#include "WheelTimer.h"


const unsigned int WheelTimer::FIRST_WHEEL_SIZE = 256;
const unsigned int WheelTimer::OTHER_WHEEL_SIZE = 64;

WheelTimer::WheelTimer(const Milliseconds tickDuration, const Milliseconds maxTimeout) : tickDuration(tickDuration) {
    auto wheelNumber = computeWheelNumber(maxTimeout);
    createWheels(wheelNumber);
}

void WheelTimer::createWheels(const unsigned int wheelNumber) {
    Wheel<TimeoutItem> wheel(FIRST_WHEEL_SIZE);
    wheels.push_back(wheel);

    for (int i = 1; i < wheelNumber; ++i) {
        Wheel<TimeoutItem> wheel(OTHER_WHEEL_SIZE);
        wheels.push_back(wheel);
    }
}

unsigned int WheelTimer::computeWheelNumber(const Milliseconds maxTimeout) const {
    unsigned int wheelNumber = 0;
    auto timePerWheel = FIRST_WHEEL_SIZE * tickDuration.count();

    auto maxTimeOutInMillis = maxTimeout.count();
    while(maxTimeOutInMillis > 0) {
        maxTimeOutInMillis -= timePerWheel * pow((double)OTHER_WHEEL_SIZE, (double)wheelNumber++);
    }
    return wheelNumber;
}

void WheelTimer::add(TimeoutItem& timeoutItem) {
    lock.lock();
    computeAndAdd(timeoutItem);
    lock.unlock();
}

void WheelTimer::computeAndAdd(TimeoutItem &timeoutItem) {
    auto timeoutValueInMillis = timeoutItem.getTimeout().count();
    auto currentWheelTime = FIRST_WHEEL_SIZE * tickDuration.count();
    auto bucketDuration = tickDuration.count();
    timeoutValueInMillis -= bucketDuration;

    for (auto it = wheels.begin(); it != wheels.end(); ++it) {
        if (timeoutValueInMillis < currentWheelTime) {
            unsigned int bucket = (unsigned int) (timeoutValueInMillis / bucketDuration);
            timeoutItem.updateTimeout(Milliseconds(timeoutValueInMillis - bucket * bucketDuration));
            (*it).add(timeoutItem, bucket + 1);
            break;
        }
        timeoutValueInMillis -= ((*it).remainingTick() - 1) * bucketDuration;
        bucketDuration = currentWheelTime;
        currentWheelTime *= OTHER_WHEEL_SIZE;
    }
}

void WheelTimer::cascade(vector<TimeoutItem> *timeoutItems) {
    for (auto it = timeoutItems->begin(); it != timeoutItems->end(); ++it) {
        if ((*it).timeOutable.isRunning()) {
            if ((*it).getTimeout().count() < tickDuration.count()) {
                (*it).timeOutable.timeout();
            } else {
                computeAndAdd(*it);
            }
        }
    }
    delete(timeoutItems);
}

void WheelTimer::tick() {
    lock.lock();
    for (int i = 0; i < wheels.size(); ++i) {
        cascade(wheels[i].nextBucket());
        if (!wheels[i].hasCascade()) {
            break;
        }
    }
    lock.unlock();
}


void WheelTimer::start() {
    isStart = true;
    timer = std::unique_ptr<std::thread>(new thread(&WheelTimer::run, this));
}

void WheelTimer::stop() {
    isStart = false;
    timer->join();
}


void WheelTimer::run() {
    const long tickDurationNano = chrono::duration_cast<chrono::nanoseconds>(tickDuration).count();
    long delta = chrono::high_resolution_clock::now().time_since_epoch().count() + tickDurationNano;

    while (isStart) {
        long now = chrono::high_resolution_clock::now().time_since_epoch().count();

        if (now >= delta) {
            delta += tickDurationNano;
            tick();
        }
    }
}


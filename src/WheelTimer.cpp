//
// Created by stan on 22/11/16.
//

#include <cmath>
#include "WheelTimer.h"

using namespace std;

const unsigned int WheelTimer::FIRST_WHEEL_SIZE = 256;
const unsigned int WheelTimer::OTHER_WHEEL_SIZE = 64;

WheelTimer::WheelTimer(const Milliseconds tickDuration, const Milliseconds maxTimeout) : tickDurationInMillis(tickDuration) {
    auto wheelNumber = computeWheelNumber(maxTimeout);
    createWheels(wheelNumber);
}

void WheelTimer::createWheels(unsigned int wheelNumber) {
    Wheel<TimeoutItem> wheel(FIRST_WHEEL_SIZE);
    wheels.push_back(wheel);

    for (int i = 1; i < wheelNumber; ++i) {
        Wheel<TimeoutItem> wheel(OTHER_WHEEL_SIZE);
        wheels.push_back(wheel);
    }
}

unsigned int WheelTimer::computeWheelNumber(const Milliseconds maxTimeout) const {
    unsigned int wheelNumber = 0;
    auto timePerWheel = FIRST_WHEEL_SIZE * tickDurationInMillis.count();

    auto maxTimeOutInMillis = maxTimeout.count();
    while(maxTimeOutInMillis > 0) {
        maxTimeOutInMillis -= timePerWheel * pow((double)OTHER_WHEEL_SIZE, (double)wheelNumber++);
    }
    return wheelNumber;
}

void WheelTimer::add(TimeoutItem& timeoutItem) {
    auto timeoutValueInMillis = timeoutItem.getTimeout().count();
    auto currentWheelTime = FIRST_WHEEL_SIZE * tickDurationInMillis.count();
    auto bucketDuration = tickDurationInMillis.count();
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

void WheelTimer::tick() {

}

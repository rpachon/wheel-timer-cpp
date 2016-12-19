//
// Created by stan on 19/12/16.
//

#include "TimeoutItem.h"

TimeoutItem::TimeoutItem(const TimeOutable& timeOutable, chrono::milliseconds timeout) : timeOutable(timeOutable), timeout(timeout) {
}

chrono::milliseconds TimeoutItem::getTimeout() {
    return timeout;
}

void TimeoutItem::updateTimeout(chrono::milliseconds timeout) {
    this->timeout = timeout;
}

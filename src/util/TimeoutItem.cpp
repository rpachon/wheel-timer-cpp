#include "TimeoutItem.h"

TimeoutItem::TimeoutItem(const TimeOutable& timeOutable, const Milliseconds timeout) : timeOutable(timeOutable), timeout(timeout) {
}

const std::chrono::milliseconds & TimeoutItem::getTimeout() const noexcept {
    return timeout;
}

void TimeoutItem::updateTimeout(const Milliseconds &timeout) noexcept {
    this->timeout = timeout;
}

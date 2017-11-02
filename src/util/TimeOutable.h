#ifndef WHEEL_TIMER_TIMEOUTABLE_H
#define WHEEL_TIMER_TIMEOUTABLE_H


class TimeOutable {
public:
    virtual void timeout() const = 0;
    virtual bool isRunning() const = 0;
};


#endif //WHEEL_TIMER_TIMEOUTABLE_H

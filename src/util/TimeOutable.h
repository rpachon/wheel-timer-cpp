//
// Created by stan on 19/12/16.
//

#ifndef WHEEL_TIMER_TIMEOUTABLE_H
#define WHEEL_TIMER_TIMEOUTABLE_H


class TimeOutable {
public:
    virtual void timeout() = 0;
    virtual bool isRunning() = 0;
};


#endif //WHEEL_TIMER_TIMEOUTABLE_H

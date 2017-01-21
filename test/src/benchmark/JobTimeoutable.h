#ifndef WHEEL_TIMER_JOBTIMEOUTABLE_H
#define WHEEL_TIMER_JOBTIMEOUTABLE_H


#include <ctime>
#include "../../../src/util/TimeOutable.h"

using namespace std;

class JobTimeoutable : public TimeOutable {

public:
    const std::chrono::time_point<chrono::high_resolution_clock> start;
    JobTimeoutable();

    virtual void timeout();
    virtual bool isRunning();

};


#endif //WHEEL_TIMER_JOBTIMEOUTABLE_H

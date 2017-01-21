#ifndef WHEEL_TIMER_BENCHMARKTIMEOUT_H
#define WHEEL_TIMER_BENCHMARKTIMEOUT_H

#include <chrono>
#include "../../../src/WheelTimer.h"
#include "JobTimeoutable.h"

using namespace std;

class BenchmarkTimeout {

public:
    virtual ~BenchmarkTimeout();
    BenchmarkTimeout();
    static std::vector<chrono::milliseconds> times;

private:
    WheelTimer *wheelTimer;

    vector<TimeoutItem*> items;
    vector<JobTimeoutable*> jobs;
    int nbr = 0;

    void computeLatency() const;

    void addJobs(int iteration);

    void waitForAllJobsReturn() const;

    void freeMemory() const;
};


#endif //WHEEL_TIMER_BENCHMARKTIMEOUT_H

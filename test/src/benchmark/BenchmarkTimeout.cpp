#include <thread>
#include <iostream>
#include <climits>
#include "BenchmarkTimeout.h"

std::vector<chrono::milliseconds> BenchmarkTimeout::times;

BenchmarkTimeout::~BenchmarkTimeout() {
    delete wheelTimer;
}

BenchmarkTimeout::BenchmarkTimeout() {
    wheelTimer = new WheelTimer(chrono::milliseconds(5), chrono::minutes(2));
    wheelTimer->start();
    this_thread::sleep_for(chrono::seconds(2));

    int iteration = 1'000'000;
    addJobs(iteration);

    waitForAllJobsReturn();

    wheelTimer->stop();
    freeMemory();

    computeLatency();
}

void BenchmarkTimeout::freeMemory() const {
    for (int i =0; i < items.size(); ++i) {
        delete items[i];
        delete jobs[i];
    }
}

void BenchmarkTimeout::waitForAllJobsReturn() const {
    while (nbr != times.size()) {
        this_thread::sleep_for(chrono::seconds(5));
    }
}

void BenchmarkTimeout::addJobs(int iteration) {
    auto time1 = chrono::high_resolution_clock().now();
    while (--iteration >= 0) {
        JobTimeoutable *job = new JobTimeoutable;
        TimeoutItem *item = new TimeoutItem(*job, chrono::seconds(60));
        wheelTimer->add(*item);
        items.push_back(item);
        jobs.push_back(job);
        ++nbr;
    }
    auto time2 = chrono::high_resolution_clock().now();
    cout << "Time to add Jobs: " << chrono::duration_cast<chrono::milliseconds>(time2 - time1).count() << endl;
}

void BenchmarkTimeout::computeLatency() const {
    long max = 0, min = LONG_MAX;
    for (int j = 0; j < times.size(); ++j) {
        if (times[j].count() > max) max = times[j].count();
        if (times[j].count() < min) min = times[j].count();
    }
    cout << "max: " << max << endl;
    cout << "min: " << min << endl;
}


int main() {
    BenchmarkTimeout bench;
}
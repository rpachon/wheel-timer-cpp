#include <chrono>
#include <iostream>
#include "JobTimeoutable.h"
#include "BenchmarkTimeout.h"

JobTimeoutable::JobTimeoutable() : start(std::chrono::high_resolution_clock::now()) {
}

void JobTimeoutable::timeout() const {
    const std::chrono::time_point<chrono::high_resolution_clock> &end = chrono::high_resolution_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(end - start);
    BenchmarkTimeout::times.push_back(time);
}

bool JobTimeoutable::isRunning() const {
    return true;
}

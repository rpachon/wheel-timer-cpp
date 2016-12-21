//
// Created by stan on 22/11/16.
//

#include <gtest/gtest.h>
#include "../../src/WheelTimer.h"
#include "util/MockTimeOutable.h"

using ::testing::Return;

TEST(WheelTimer, should_create_wheels_function_of_max_timeout) {
    // Given
    chrono::milliseconds tickDuration(1);
    std::map<long long, int> valueToTest;
    valueToTest.insert(std::pair<long, int>(148, 1));
    valueToTest.insert(std::pair<long, int>(11000, 2));
    valueToTest.insert(std::pair<long, int>(120000, 3));
    valueToTest.insert(std::pair<long, int>(2000000, 4));

    for (auto it = valueToTest.begin(); it != valueToTest.end(); it++) {
        // Given
        chrono::milliseconds maxTimeout(it->first);

        // When
        WheelTimer timer(tickDuration, maxTimeout);

        // Then
        EXPECT_EQ(timer.wheels.size(), it->second);
    }
}

TEST(WheelTimer, should_add_an_item_in_the_first_wheel_when_timeout_between_1_and_256_multiply_tick_duration) {
    // Given
    chrono::milliseconds tickDuration(1);
    chrono::milliseconds maxTimeout(120000);
    WheelTimer timer(tickDuration, maxTimeout);
    std::vector<int> valuesToTest;
    valuesToTest.push_back(1);
    valuesToTest.push_back(2);
    valuesToTest.push_back(10);
    valuesToTest.push_back(125);
    valuesToTest.push_back(256);

    MockTimeOutable mockTimeOutable;

    for (auto it = valuesToTest.begin(); it != valuesToTest.end(); it++) {
        // Given
        TimeoutItem timeoutItem(mockTimeOutable, chrono::milliseconds(*it));

        // When
        timer.add(timeoutItem);

        // Then
        auto wheel = timer.wheels.begin();
        for (int i = 0; i < *it-1; ++i) {
            auto result = (*wheel).nextBucket();
            EXPECT_EQ(result.size(), 0);
        }
        auto result = (*wheel).nextBucket();
        EXPECT_EQ(result.size(), 1);
    }
}

TEST(WheelTimer, should_add_an_item_in_the_second_wheel_when_timeout_between_257_and_16384_multiply_tick_duration) {
// Given
    chrono::milliseconds tickDuration(1);
    chrono::milliseconds maxTimeout(120000);
    WheelTimer timer(tickDuration, maxTimeout);

    std::map<long long, int> valueToTest;
    valueToTest.insert(std::pair<long, int>(257, 1));
    valueToTest.insert(std::pair<long, int>(16000, 62));
    valueToTest.insert(std::pair<long, int>(16384, 64));

    MockTimeOutable mockTimeOutable;

    for (auto it = valueToTest.begin(); it != valueToTest.end() ; ++it) {
        // Given
        TimeoutItem timeoutItem(mockTimeOutable, chrono::milliseconds((*it).first));

        // When
        timer.add(timeoutItem);

        // Then
        auto wheel = timer.wheels.begin()+1;
        for (int i = 0; i < (*it).second-1; ++i) {
            auto result = (*wheel).nextBucket();
            EXPECT_EQ(result.size(), 0);
        }
        auto result = (*wheel).nextBucket();
        EXPECT_EQ(result.size(), 1);
    }
}

TEST(WheelTimer, should_add_an_item_in_the_third_wheel_timeout_between_16641_and_1065216_multiply_tick_duration) {
// Given
    chrono::milliseconds tickDuration(1);
    chrono::milliseconds maxTimeout(120000);
    WheelTimer timer(tickDuration, maxTimeout);

    std::map<long long, int> valueToTest;
    valueToTest.insert(std::pair<long, int>(16641, 0));
    valueToTest.insert(std::pair<long, int>(120000, 7));
    valueToTest.insert(std::pair<long, int>(1048576, 64));

    MockTimeOutable mockTimeOutable;

    for (auto it = valueToTest.begin(); it != valueToTest.end() ; ++it) {
        // Given
        TimeoutItem timeoutItem(mockTimeOutable, chrono::milliseconds((*it).first));

        // When
        timer.add(timeoutItem);

        // Then
        auto wheel = timer.wheels.begin()+2;
        for (int i = 0; i < (*it).second-1; ++i) {
            auto result = (*wheel).nextBucket();
            EXPECT_EQ(result.size(), 0);
        }
        auto result = (*wheel).nextBucket();
        EXPECT_EQ(result.size(), 1);
    }

}

TEST(WheelTimer, should_add_an_item_in_first_wheel_when_wheel_has_already_tick) {
// Given
    chrono::milliseconds tickDuration(1);
    chrono::milliseconds maxTimeout(120000);
    WheelTimer timer(tickDuration, maxTimeout);
    auto wheel = timer.wheels.begin();
    for (int j = 0; j < 120; ++j) {
        (*wheel).nextBucket();
    }

    std::vector<int> valuesToTest;
    valuesToTest.push_back(1);
    valuesToTest.push_back(2);
    valuesToTest.push_back(10);
    valuesToTest.push_back(125);
    valuesToTest.push_back(230);
    valuesToTest.push_back(250);
    valuesToTest.push_back(256);

    MockTimeOutable mockTimeOutable;

    for (auto it = valuesToTest.begin(); it != valuesToTest.end(); it++) {
        // Given
        TimeoutItem timeoutItem(mockTimeOutable, chrono::milliseconds(*it));

        // When
        timer.add(timeoutItem);

        // Then
        auto wheel = timer.wheels.begin();
        for (int i = 0; i < *it-1; ++i) {
            auto result = (*wheel).nextBucket();
            EXPECT_EQ(result.size(), 0);
        }
        auto result = (*wheel).nextBucket();
        EXPECT_EQ(result.size(), 1);
    }
}

TEST(WheelTimer, should_add_an_item_in_second_wheel_when_wheels_has_already_tick) {
// Given
    chrono::milliseconds tickDuration(1);
    chrono::milliseconds maxTimeout(120000);
    WheelTimer timer(tickDuration, maxTimeout);
    auto wheel = timer.wheels.begin()+1;
    for (int j = 0; j < 60; ++j) {
        (*wheel).nextBucket();
    }

    MockTimeOutable mockTimeOutable;

    TimeoutItem timeoutItem(mockTimeOutable, chrono::milliseconds(15000));

// When
    timer.add(timeoutItem);

// Then
    for (int i = 0; i < 57; ++i) {
        auto result = (*wheel).nextBucket();
        EXPECT_EQ(result.size(), 0);
    }
    auto result = (*wheel).nextBucket();
    EXPECT_EQ(result.size(), 1);
}

TEST(WheelTimer, should_add_an_item_in_third_wheel_when_wheels_has_already_tick) {
// Given
    chrono::milliseconds tickDuration(1);
    chrono::milliseconds maxTimeout(120000);
    WheelTimer timer(tickDuration, maxTimeout);
    auto wheel = timer.wheels.begin()+2;
    for (int j = 0; j < 10; ++j) {
        (*wheel).nextBucket();
    }

    MockTimeOutable mockTimeOutable;

    TimeoutItem timeoutItem(mockTimeOutable, chrono::milliseconds(1000000));

// When
    timer.add(timeoutItem);

// Then
    for (int i = 0; i < 60; ++i) {
        auto result = (*wheel).nextBucket();
        EXPECT_EQ(result.size(), 0);
    }
    auto result = (*wheel).nextBucket();
    EXPECT_EQ(result.size(), 1);
}

TEST(WheelTimer, should_compute_remaining_time_when_adding_in_wheels) {
// Given
    chrono::milliseconds tickDuration(1);
    chrono::milliseconds maxTimeout(120000);
    WheelTimer timer(tickDuration, maxTimeout);

    MockTimeOutable mockTimeOutable;
    TimeoutItem timeoutItem(mockTimeOutable, chrono::minutes(1));

// When
    timer.add(timeoutItem);

// Then
    auto wheel = timer.wheels.begin()+2;
    for (int i = 0; i < 2; ++i) {
        auto result = (*wheel).nextBucket();
        EXPECT_EQ(result.size(), 0);
    }
    auto result = (*wheel).nextBucket();
    auto timeoutItemToTest = result.begin();
    EXPECT_EQ((*timeoutItemToTest).getTimeout().count(), 10848);
}



TEST(WheelTimer, should_cascade_bucket_if_not_in_the_first_wheel) {
// Given
    chrono::milliseconds tickDuration(1);
    chrono::milliseconds maxTimeout(120000);
    WheelTimer timer(tickDuration, maxTimeout);

    std::vector<int> valuesToTest;
    valuesToTest.push_back(1);
    valuesToTest.push_back(2);
    valuesToTest.push_back(10);
    valuesToTest.push_back(125);
    valuesToTest.push_back(256);
    valuesToTest.push_back(257);
    valuesToTest.push_back(16000);
    valuesToTest.push_back(16384);
    valuesToTest.push_back(16640);
    valuesToTest.push_back(16641);
    valuesToTest.push_back(100000);
    valuesToTest.push_back(120000);

    MockTimeOutable mockTimeOutable;
    EXPECT_CALL(mockTimeOutable, isRunning())
                .WillRepeatedly(Return(true));

    for (auto it = valuesToTest.begin(); it != valuesToTest.end(); ++it) {
        // Given
        TimeoutItem timeoutItem(mockTimeOutable, chrono::milliseconds(*it));
        timer.add(timeoutItem);

        // When Then
        for (int i = 0; i < (*it)-1; ++i) {
            EXPECT_CALL(mockTimeOutable, timeout())
                    .Times(0);
            timer.tick();
        }

        EXPECT_CALL(mockTimeOutable, timeout())
                .Times(1);
        timer.tick();

    }
}
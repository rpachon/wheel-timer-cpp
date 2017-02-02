#include <gtest/gtest.h>
#include "../../src/WheelTimer.h"
#include "util/MockTimeOutable.h"

using ::testing::Return;


TEST(WheelTimer, should_timeout_all_items_with_tick_duration_at_one_ms) {
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
    valuesToTest.push_back(1048570);

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

TEST(WheelTimer, should_timeout_all_items_with_tick_duration_at_thirty_ms) {
// Given
    chrono::milliseconds tickDuration(30);
    chrono::minutes maxTimeout(5);
    WheelTimer timer(tickDuration, maxTimeout);

    std::map<int, int> valuesToTests;
    valuesToTests[32] = 1;
    valuesToTests[75] = 2;
    valuesToTests[7680] = 256;
    valuesToTests[7709] = 256;
    valuesToTests[7710] = 257;
    valuesToTests[125000] = 4166;

    MockTimeOutable mockTimeOutable;
    EXPECT_CALL(mockTimeOutable, isRunning())
            .WillRepeatedly(Return(true));

    for (auto it = valuesToTests.begin(); it != valuesToTests.end(); ++it) {
        // Given
        TimeoutItem timeoutItem(mockTimeOutable, chrono::milliseconds((*it).first));
        timer.add(timeoutItem);

        // When Then
        for (int i = 0; i < (*it).second-1; ++i) {
            EXPECT_CALL(mockTimeOutable, timeout())
                    .Times(0);
            timer.tick();
        }

        EXPECT_CALL(mockTimeOutable, timeout())
                .Times(1);
        timer.tick();

    }
}
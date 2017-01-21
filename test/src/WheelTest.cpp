#include <gtest/gtest.h>
#include "../../src/Wheel.h"

using namespace std;

TEST(Wheel, should_throw_exception_if_size_is_less_or_equals_to_0) {
// Given
// When
// Then
    EXPECT_THROW(Wheel<long>(0), std::runtime_error);
}

TEST(Wheel, should_add_item_of_different_types_in_bucket) {
    // Given
    Wheel<long> wheelLong(10);
    Wheel<std::string> wheelString(15);

    // When
    long item1 = 4L;
    wheelLong.add(item1, 5);
    string item2 = "test";
    wheelString.add(item2, 8);

    // Then
    EXPECT_EQ(wheelLong.wheel[5][0], 4);
    EXPECT_EQ(wheelString.wheel[8][0], "test");
}

TEST(Wheel, should_add_many_items_in_the_same_bucket) {
// Given
    Wheel<long> wheel(256);

// When
    long item1 = 5;
    wheel.add(item1, 123);
    long item2 = 7;
    wheel.add(item2, 123);

// Then
    auto results = wheel.wheel[123];
    EXPECT_EQ(results[0], 5);
    EXPECT_EQ(results[1], 7);
}

TEST(Wheel, should_get_next_bucket) {
// Given
    Wheel<string> wheel(5);
    string item0 = "test0";
    string item1 = "test1";
    string item2 = "test2";
    string item3 = "test3";
    string item4 = "test4";
    string item5 = "test5";
    wheel.add(item0, 0);
    wheel.add(item1, 1);
    wheel.add(item2, 2);
    wheel.add(item3, 3);
    wheel.add(item4, 4);
    wheel.add(item5, 4);

// When
    auto results = wheel.nextBucket();
// Then
    EXPECT_EQ(results->at(0), "test1");

// When
    results = wheel.nextBucket();
// Then
    EXPECT_EQ(results->at(0), "test2");

// When
    results = wheel.nextBucket();
// Then
    EXPECT_EQ(results->at(0), "test3");

// When
    results = wheel.nextBucket();
// Then
    EXPECT_EQ(results->at(0), "test4");
    EXPECT_EQ(results->at(1), "test5");

// When
    results = wheel.nextBucket();
// Then
    EXPECT_EQ(results->at(0), "test0");

}

TEST(Wheel, should_add_item_when_wheel_is_not_initialized) {
// Given
    Wheel<long> wheel(5);
    wheel.nextBucket();
    wheel.nextBucket();

// When
    long item = 2L;
    wheel.add(item, 1);

// Then
    EXPECT_EQ(wheel.nextBucket()->at(0), 2);
}

TEST(Wheel, should_rotate_when_getting_more_than_size_next_bucket) {
// Given
    Wheel<string> wheel(5);
    for (int i = 0; i < 4; ++i) {
        wheel.nextBucket();
    }
    string item = "test";
    wheel.add(item, 2);
    wheel.nextBucket();

// When
    auto results = wheel.nextBucket();

// Then
    EXPECT_EQ(results->at(0), "test");
}

TEST(Wheel, should_clear_bucket_when_bucket_is_ridden) {
// Given
    Wheel<long> wheel(5);
    long item = 34L;
    wheel.add(item, 2);

    for (int i = 0; i < 10; ++i) {
// When
        auto expectedVector = wheel.nextBucket();
// Then
        if (i == 1) EXPECT_EQ(expectedVector->at(0), 34);
        else
            EXPECT_EQ(expectedVector->size(), 0);
    }

}

TEST(Wheel, should_cascade_when_making_complete_turn) {
// Given
    Wheel<string> wheel(5);

// When
    for (int i = 0; i < 5; ++i) {
        wheel.nextBucket();
    }

// Then
    EXPECT_TRUE(wheel.hasCascade());
}

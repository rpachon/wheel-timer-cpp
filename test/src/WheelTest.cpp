//
// Created by stan on 09/11/16.
//

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
    wheelLong.add(4L, 5);
    wheelString.add("test", 8);

    // Then
    EXPECT_EQ(wheelLong.get(5)[0], 4);
    EXPECT_EQ(wheelString.get(8)[0], "test");
}

TEST(Wheel, should_add_many_items_in_the_same_bucket) {
// Given
    Wheel<long> wheel(256);

// When
    wheel.add(5, 123);
    wheel.add(7, 123);

// Then
    auto results = wheel.get(123);
    EXPECT_EQ(results[0], 5);
    EXPECT_EQ(results[1], 7);
}

TEST(Wheel, should_get_next_bucket) {
// Given
    Wheel<string> wheel(5);
    wheel.add("test0", 0);
    wheel.add("test1", 1);
    wheel.add("test2", 2);
    wheel.add("test3", 3);
    wheel.add("test4", 4);
    wheel.add("test5", 4);

// When
    auto results = wheel.nextBucket();
// Then
    EXPECT_EQ(results[0], "test1");

// When
    results = wheel.nextBucket();
// Then
    EXPECT_EQ(results[0], "test2");

// When
    results = wheel.nextBucket();
// Then
    EXPECT_EQ(results[0], "test3");

// When
    results = wheel.nextBucket();
// Then
    EXPECT_EQ(results[0], "test4");
    EXPECT_EQ(results[1], "test5");

// When
    results = wheel.nextBucket();
// Then
    EXPECT_EQ(results[0], "test0");

}

TEST(Wheel, should_add_item_when_wheel_is_not_initialized) {
// Given
    Wheel<long> wheel(5);
    wheel.nextBucket();
    wheel.nextBucket();

// When
    wheel.add(2, 1);

// Then
    EXPECT_EQ(wheel.nextBucket()[0], 2);
}

TEST(Wheel, should_rotate_when_getting_more_than_size_next_bucket) {
// Given
    Wheel<string> wheel(5);
    for (int i = 0; i < 4; ++i) {
        wheel.nextBucket();
    }
    wheel.add("test", 2);
    wheel.nextBucket();

// When
    auto results = wheel.nextBucket();

// Then
    EXPECT_EQ(results[0], "test");
}

TEST(Wheel, should_clear_bucket_when_bucket_is_ridden) {
// Given
    Wheel<long> wheel(5);
    wheel.add(34, 2);

    for (int i = 0; i < 10; ++i) {
// When
        auto expectedVector = wheel.nextBucket();
// Then
        if (i == 1) EXPECT_EQ(expectedVector[0], 34);
        else
            EXPECT_EQ(expectedVector.size(), 0);
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

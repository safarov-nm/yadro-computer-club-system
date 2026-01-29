#include "time.hpp"
#include "input_validator.hpp"
#include <gtest/gtest.h>

TEST(TimeTest, ConstructorFromString) {
    Time t1("09:30");
    EXPECT_EQ(t1.toString(), "09:30");
}

TEST(TimeTest, ConstructorFromHoursMinutes) {
    Time t1(10, 30);
    EXPECT_EQ(t1.toString(), "10:30");
    
    Time t2(0, 5);
    EXPECT_EQ(t2.toString(), "00:05");
    
    Time t3(23, 59);
    EXPECT_EQ(t3.toString(), "23:59");
}

TEST(TimeTest, IsCorrectTimeValidation) {
    EXPECT_TRUE(isCorrectTime("09:30"));
    EXPECT_TRUE(isCorrectTime("15:15"));
    EXPECT_FALSE(isCorrectTime("25:00"));
    EXPECT_FALSE(isCorrectTime("12:60"));
    EXPECT_FALSE(isCorrectTime("-08:10"));
}

TEST(TimeTest, TimeComparison) {
    Time t1("09:00");
    Time t2("10:00");

    Time t3("12:00");
    Time t4("12:00");
    EXPECT_TRUE(t1 < t2);
    EXPECT_TRUE(t3 <= t4);
    EXPECT_FALSE(t2 < t1);
}

TEST(TimeTest, ToMinutesConversion) {
    Time t1("01:30");
    EXPECT_EQ(t1.toMinutes(), 90);
    
    Time t2("00:00");
    EXPECT_EQ(t2.toMinutes(), 0);
    
    Time t3("23:59");
    EXPECT_EQ(t3.toMinutes(), 23*60 + 59);
}

TEST(TimeTest, DifferenceInMinutes) {
    Time t1("10:00");
    Time t2("11:30");
    
    EXPECT_EQ(t2.differenceInMinutes(t1), 90);
    
    Time t3("10:00");
    EXPECT_EQ(t3.differenceInMinutes(t3), 0);
}

TEST(TimeTest, FromMinutes) {
    Time t1 = Time::fromMinutes(90);
    EXPECT_EQ(t1.toString(), "01:30");
    
    Time t2 = Time::fromMinutes(0);
    EXPECT_EQ(t2.toString(), "00:00");
    
    Time t3 = Time::fromMinutes(1439);
    EXPECT_EQ(t3.toString(), "23:59");
}

TEST(TimeTest, RevenueCalculation) {
    Time t1(2, 30);
    EXPECT_EQ(t1.calculateRevenue(10), 30);

    Time t2(1, 0);
    EXPECT_EQ(t2.calculateRevenue(10), 10);

    Time t3(0, 1);
    EXPECT_EQ(t3.calculateRevenue(10), 10);
}
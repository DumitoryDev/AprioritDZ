#include "pch.h"

TEST(YearHelper, YearHelper_CheckZeroYear) {
	EXPECT_TRUE(YearHelper::IsLeapYear(0));
}

TEST(YearHelper, YearHelper_CheckNoLeapYear) {
	EXPECT_FALSE(YearHelper::IsLeapYear(2011));
}

TEST(YearHelper, YearHelper_Check1900Year) {
	EXPECT_FALSE(YearHelper::IsLeapYear(1900));
}

TEST(YearHelper, YearHelper_CheckLeapYear) {
	EXPECT_TRUE(YearHelper::IsLeapYear(2020));
}
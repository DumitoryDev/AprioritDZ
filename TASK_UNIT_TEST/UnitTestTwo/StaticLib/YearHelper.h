#pragma once
class YearHelper
{
public:
	~YearHelper(void) = delete;
	YearHelper(void) = delete;
	YearHelper(const YearHelper& other) = delete;
	YearHelper(YearHelper&& other) noexcept = delete;
	YearHelper& operator=(const YearHelper& other) = delete;
	YearHelper& operator=(YearHelper&& other) noexcept = delete;

	static bool IsLeapYear(size_t year);
	
};


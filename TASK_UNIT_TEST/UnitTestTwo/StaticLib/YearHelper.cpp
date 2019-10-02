#include "pch.h"
#include "YearHelper.h"

bool YearHelper::IsLeapYear(const size_t year)
{
	return !(year%4) && (!(year % 400) || year % 100);
}

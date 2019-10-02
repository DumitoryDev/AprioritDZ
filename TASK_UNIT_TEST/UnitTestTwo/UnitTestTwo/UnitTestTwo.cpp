// UnitTestTwo.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "YearHelper.h"
int main()
{
	try
	{
		std::cout<<std::boolalpha<<YearHelper::IsLeapYear(2000);
	}
	catch (...)
	{
	}
   
}

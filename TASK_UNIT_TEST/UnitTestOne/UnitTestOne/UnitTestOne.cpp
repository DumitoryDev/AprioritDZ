#include <iostream>
#include "framework.h"
int main(void)
{
	try
	{
		std::vector<int> data{5,5,5};
		std::cout << lib::GetProductsAmount(data,15);
	}
	catch (...)
	{
	}
    
}


#include "pch.h"
#include "framework.h"

namespace lib
{
	int GetProductsAmount(std::vector<int>& prices, int amountOfMoney) {

		std::sort(prices.begin(), prices.end());

		int sum = 0;

		for (int i = 0; i < prices.size(); ++i)

		{

			sum += prices[i];

			if (sum > amountOfMoney)

			{

				return i;

			}

		}

		return prices.size();

	}
}

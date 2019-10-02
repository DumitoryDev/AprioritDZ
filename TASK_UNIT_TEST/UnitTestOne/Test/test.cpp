#include "pch.h"


TEST(framework, GetProductsAmount_CheckTrueSortedVector) {
	std::vector<int> tmp{ 4,6,1,2 };
	lib::GetProductsAmount(tmp, 100);
	EXPECT_TRUE(std::is_sorted(tmp.cbegin(), tmp.cend()));
}

TEST(framework, GetProductsAmount_CheckEqualAmountOfMoney) {

	EXPECT_EQ(lib::GetProductsAmount(std::vector<int>{5, 5, 5}, 15), 3);
}

TEST(framework, GetProductsAmount_CheckZeroProducts) {
	EXPECT_EQ(lib::GetProductsAmount(std::vector<int>(), 2), 0);
}

TEST(framework, GetProductsAmount_CheckZeroMoney) {
	EXPECT_EQ(lib::GetProductsAmount(std::vector<int>({ 1,2,3 }), 0), 0);
}

TEST(framework, GetProductsAmount_CheckAmountMoneyLessProducts) {
	EXPECT_EQ(lib::GetProductsAmount(std::vector<int>{5, 5, 5}, 5), 1);
}

TEST(framework, GetProductsAmount_CheckAmountMoneyGreatProducts) {
	EXPECT_EQ(lib::GetProductsAmount(std::vector<int>{5, 5, 5}, 20), 3);
}

TEST(framework, GetProductsAmount_CheckMoneyNegativNumber) {
	EXPECT_EQ(lib::GetProductsAmount(std::vector<int>{-15, -20}, -1), 2);
}


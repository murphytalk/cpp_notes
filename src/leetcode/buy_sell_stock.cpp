#include "catch.hpp"
#include <bitset>
#include <vector>

using namespace std;

namespace LeetCode {

/*
https://leetcode.com/problems/best-time-to-buy-and-sell-stock/#/description

Say you have an array for which the ith element is the price of a given stock on day i.

If you were only permitted to complete at most one transaction (ie, buy one and sell one share of the stock), 
design an algorithm to find the maximum profit.

Example 1:

Input: [7, 1, 5, 3, 6, 4]
Output: 5

max. difference = 6-1 = 5 (not 7-1 = 6, as selling price needs to be larger than buying price)


Example 2:

Input: [7, 6, 4, 3, 1]
Output: 0

In this case, no transaction is done, i.e. max profit = 0.
*/
static int max_profit_I(const vector<int>& prices){
    const auto N = prices.size();
#if 0
    vector<int> max_profit(N); 
    int overall_max = 0;
    for (auto i = 0; i < N;++i) {
        const int& price = prices[i];
        int max = 0;
        for (auto k = 0; k < i; ++k) {
            if (prices[k] < price) {
                int profit = max_profit[k] + price - prices[k];
                if (profit > max) max = profit;
            }
        }
        max_profit[i] = max;
        if (max > overall_max) overall_max = max;
    }
    return  overall_max;
#else
    if (N <= 1) return 0;

    int min_price = min(prices[0], prices[1]);
    int profit = prices[1]-prices[0];
    for (auto i = 2; i < N;++i) {
        const int& price = prices[i];
        profit = max(profit, price - min_price);
        if (price < min_price) min_price = price;
    }
    return profit<0?0:profit;
#endif
}

TEST_CASE("best time to bull and sell stock I : 7, 1, 5, 3, 6, 4", "[leetcode]") {
    const vector<int> prices = { 7, 1, 5, 3, 6, 4 };
    REQUIRE(max_profit_I(prices) == 5);
}


TEST_CASE("best time to bull and sell stock I : 7, 6, 4, 3, 1", "[leetcode]") {
    const vector<int> prices = { 7, 6, 4, 3, 1};
    REQUIRE(max_profit_I(prices) == 0);
}

/*
https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/#/description

Say you have an array for which the ith element is the price of a given stock on day i.

Design an algorithm to find the maximum profit. You may complete as many transactions as you like 
(ie, buy one and sell one share of the stock multiple times). 
However, you may not engage in multiple transactions at the same time (ie, you must sell the stock before you buy again).
*/

static int max_profit_II(const vector<int>& prices) {
    const auto N = prices.size();

}


}
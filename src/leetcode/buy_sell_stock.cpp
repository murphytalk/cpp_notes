#include "catch.hpp"
#include <bitset>
#include <vector>
#include <iterator>
//#include <iostream>
//#include <algorithm>
#include "src/utils/utils.h"

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
    if (N <= 1) return 0;

    int min_price = min(prices[0], prices[1]);
    int profit = prices[1]-prices[0];
    for (auto i = 2; i < N;++i) {
        const int& price = prices[i];
        profit = max(profit, price - min_price);
        if (price < min_price) min_price = price;
    }
    return profit<0?0:profit;
}

TEST_CASE("Stock max profit I : 7, 1, 5, 3, 6, 4", "[leetcode]") {
    const vector<int> prices = { 7, 1, 5, 3, 6, 4 };
    REQUIRE(max_profit_I(prices) == 5);
}


TEST_CASE("Stock max profit I : 7, 6, 4, 3, 1", "[leetcode]") {
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

namespace
{
static int max_profit_II_recursive(vector<int>::iterator begin, vector<int>::iterator end)
{
    if (begin == end) return 0;
    int max = 0;
    for(auto buy = begin; buy!=end; ++buy){
        for (auto sell = std::next(buy); sell!=end; ++sell){
            int profit = *sell - *buy;
            if ( profit > 0){
                max = std::max(max, profit + max_profit_II_recursive(std::next(sell), end));
            }
        }
    }
    return max;
}

static int peak_valley(vector<int>& prices){
    int profit = 0;
    for (auto valley = prices.begin(), peak = std::next(valley); peak != prices.end() && valley != prices.end();){
        if (*valley >= *peak){
            ++valley;
            ++peak;
        }
        else{
            auto next_peak = std::next(peak);
            for(;next_peak != prices.end() && *next_peak > *peak; ++next_peak);
            peak = std::prev(next_peak);
            profit += *peak - *valley;
            valley = next_peak;
            peak = std::next(valley);
        }
    }
    return profit;
}

static int max_profit_II(vector<int>& prices) {
    
}

TEST_CASE("Stock max profit II : 1,2,7,4", "[leetcode]") {
    vector<int> prices = { 1,2,7,4 };
    REQUIRE(max_profit_II_recursive(prices.begin(), prices.end()) == 6);
    REQUIRE(peak_valley(prices) == 6);
}

TEST_CASE("Stock max profit II : 7, 1, 5, 3, 6, 4", "[leetcode]") {
    vector<int> prices = { 7, 1, 5, 3, 6, 4 };
    REQUIRE(max_profit_II_recursive(prices.begin(), prices.end()) == 7);
    REQUIRE(peak_valley(prices) == 7);
}

TEST_CASE("Stock max profit II : 1, 2, 3, 4, 5", "[leetcode]") {
    vector<int> prices = { 1, 2, 3, 4, 5 };
    REQUIRE(max_profit_II_recursive(prices.begin(), prices.end()) == 4);
    REQUIRE(peak_valley(prices) == 4);
}

TEST_CASE("Stock max profit II : 7, 6, 4, 3, 1", "[leetcode]") {
    vector<int> prices = { 7, 6, 4, 3, 1 };
    REQUIRE(max_profit_II_recursive(prices.begin(), prices.end()) == 0);
    REQUIRE(peak_valley(prices) == 0);
}

TEST_CASE("Stock max profit II : big", "[leetcode]") {
    std::ifstream f(Utils::get_data_file_path("buy_sell_stock.txt"), ifstream::in);
    typedef std::istream_iterator<Utils::comma_sep<int>> istrit;
    std::vector<int> //prices = Utils::read_to_container<std::vector<int>, int>(s);
        prices((istrit(f)), istrit());
    //typedef std::ostream_iterator<int> ostrit;
    //std::copy(prices.begin(), prices.end(), ostrit(std::cout, ";"));

    int a1 = 0;
    SECTION("Recursive"){
        //a1 = max_profit_II_recursive(prices.begin(), prices.end());
    }

    int a2 = 0;
    SECTION("Peak Valley"){
        a2 = peak_valley(prices);
    }

    
}

}


}
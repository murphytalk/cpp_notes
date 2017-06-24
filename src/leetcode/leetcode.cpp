#include "catch.hpp"
#include <string>
#include <vector>
#include <array>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <memory>
#include <iterator>

using namespace std;

namespace LeetCode{
    
/*
  https://leetcode.com/problems/search-for-a-range/

  Given a sorted array of integers, find the starting and ending position of a given target value.

  Your algorithm's runtime complexity must be in the order of O(log n).

  If the target is not found in the array, return [-1, -1].

  For example,
  Given [5, 7, 7, 8, 8, 10] and target value 8, return [3, 4].
*/
class SearchRange {
public:
	vector<int> searchRange(vector<int>& nums, int target) {
		vector<int> range;
		auto low = lower_bound(nums.begin(), nums.end(), target);
		if (low == nums.end() || *low != target) {
			range.push_back(-1);
			range.push_back(-1);
		}
		else {
			range.push_back((int)distance(nums.begin(),low));
			auto up = upper_bound(low, nums.end(), target);
			range.push_back((int)distance(nums.begin(),up)-1);
		}
		return range;
	}
};

TEST_CASE("Search Range: no duplication test case", "[leetcode]") {
	vector<int> nums = { 0,1,2,3,4,5,6,7,8,9,10,11 };
	vector<int> r;
	SearchRange s;

	SECTION("too small") {
		r = s.searchRange(nums, -1);
		REQUIRE(r[0] == -1);
		REQUIRE(r[1] == -1);
	}

	SECTION("too big") {
		r = s.searchRange(nums, 100);
		REQUIRE(r[0] == -1);
		REQUIRE(r[1] == -1);
	}

	SECTION("smallest") {
		r = s.searchRange(nums, 0);
		REQUIRE(r[0] == 0);
		REQUIRE(r[1] == 0);
	}

	SECTION("biggest") {
		r = s.searchRange(nums, 11);
		REQUIRE(r[0] == 11);
		REQUIRE(r[1] == 11);
	}

}

TEST_CASE("Search Range: duplication test case", "[leetcode]") {
	vector<int> r;
	SearchRange s;

	SECTION("all same as target") {
		vector<int> nums = { 10,10,10,10,10,10 };
		r = s.searchRange(nums, 10);
		REQUIRE(r[0] ==  0);
		REQUIRE(r[1] ==  5);
	}

	SECTION("partial duplication") {
		vector<int> nums = { 5, 7, 7, 8, 8, 10 };
		r = s.searchRange(nums, 8);
		REQUIRE(r[0] == 3);
		REQUIRE(r[1] == 4);
	}
}

/*
https://leetcode.com/problems/add-two-numbers/

You are given two non-empty linked lists representing two non-negative integers. 
The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

You may assume the two numbers do not contain any leading zero, except the number 0 itself.

Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
*/
class AddTwoNumbers {
public:
	struct ListNode {
		int val;
		unique_ptr<ListNode> next;
		ListNode(int x, ListNode *n = nullptr) : val(x), next(n) {}
		//~ListNode() { LOG << val << endl; }
	};

	inline void next(ListNode*& l, int& v) const {
		if (l == nullptr) {
			v = 0;
		}
		else {
			v = l->val;
			l = l->next.get();
		}
	}

	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) const {
		ListNode *p1 = l1, *p2 = l2;
		ListNode *l = nullptr, *dummy_head = new ListNode(0), *last=dummy_head;
		int v, carry = 0;
		int p1v, p2v;

		while( p1 != nullptr || p2 != nullptr){
			next(p1, p1v);
			next(p2, p2v);
			if ((v = carry + p1v + p2v) > 9) {
				l = new ListNode(v % 10);
				carry = v / 10;
			}
			else {
				l = new ListNode(v);
				carry = 0;
			}
			last->next.reset(l);
			last = l;
		}

		if ( carry > 0) {
			last->next.reset(new ListNode(carry));
		}


		ListNode* ret = dummy_head->next.get();
		dummy_head->next.release();
		delete dummy_head;
		return ret;
	}

	const vector<int> addTwoNumbersAsVec(ListNode* l1, ListNode* l2) const {
		vector<int> result;

		ListNode *l = addTwoNumbers(l1, l2);
		for (ListNode *n = l; n != nullptr; n = n->next.get()){
			result.push_back(n->val);
		}
		delete l;
		return result;
	}
};

TEST_CASE("Add Two Numbers", "[leetcode]") {
	const AddTwoNumbers test;

	SECTION("(2 -> 4 -> 3) + (5 -> 6 -> 4)") {
		AddTwoNumbers::ListNode *l1 = new AddTwoNumbers::ListNode(2, new AddTwoNumbers::ListNode(4, new AddTwoNumbers::ListNode(3)));
		AddTwoNumbers::ListNode *l2 = new AddTwoNumbers::ListNode(5, new AddTwoNumbers::ListNode(6, new AddTwoNumbers::ListNode(4)));

		const vector<int> verify{ 7,0,8 };
		REQUIRE(test.addTwoNumbersAsVec(l1, l2) == verify);
	}

	SECTION("(4 -> 3) + (5 ->6)") {
		AddTwoNumbers::ListNode *l1 = new AddTwoNumbers::ListNode(2, new AddTwoNumbers::ListNode(4, new AddTwoNumbers::ListNode(3)));
		AddTwoNumbers::ListNode *l2 = new AddTwoNumbers::ListNode(5, new AddTwoNumbers::ListNode(6));

		const vector<int> verify{7,0,4};
		REQUIRE(test.addTwoNumbersAsVec(l1,l2) == verify);
	}

	SECTION("(5) + (5)") {
    	AddTwoNumbers::ListNode *l1 = new AddTwoNumbers::ListNode(5);
    	AddTwoNumbers::ListNode *l2 = new AddTwoNumbers::ListNode(5);
    
    	const vector<int> verify{0,1};
    	REQUIRE(test.addTwoNumbersAsVec(l1,l2) == verify);
	}

	SECTION("(1) + (9 -> 9)") {
    	AddTwoNumbers::ListNode *l1 = new AddTwoNumbers::ListNode(1);
    	AddTwoNumbers::ListNode *l2 = new AddTwoNumbers::ListNode(9,new AddTwoNumbers::ListNode(9));
    
    	const vector<int> verify{0,0,1};
    	REQUIRE(test.addTwoNumbersAsVec(l1,l2) == verify);
	}

	SECTION("(9 -> 1 -> 6) + (0)") {
		AddTwoNumbers::ListNode *l1 = new AddTwoNumbers::ListNode(9, new AddTwoNumbers::ListNode(1, new AddTwoNumbers::ListNode(6)));
		AddTwoNumbers::ListNode *l2 = new AddTwoNumbers::ListNode(0);

		const vector<int> verify{ 9,1,6 };
		REQUIRE(test.addTwoNumbersAsVec(l1, l2) == verify);
	}
};



}; //namespace LeetCode


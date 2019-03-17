#!/usr/bin/env python
import os
import sys
from zipfile import ZipFile
import unittest


class MyTestCase(unittest.TestCase):
    def get_test_data(self, test_name, callback):
        """
        test data is saved under ../../data/ folder
        if test_name.txt does not exist, then the same file should be in <class name>.zip
        """
        def read_lines(fobj):
            for line in fobj:
                callback(line)

        fileDir = os.path.dirname(os.path.abspath(__file__))
        fname = os.path.join(fileDir, "../../data/{}.txt".format(test_name))
        if os.path.exists(fname):
            with open(fname) as f:
                read_lines(f)
        else:
            fname = os.path.join(fileDir, "../../data/{}.zip".format(self.__class__.__name__))
            if os.path.exists(fname):
                with ZipFile(fname) as myzip:
                    with myzip.open('{}.txt'.format(test_name)) as f:
                        read_lines(f)


class UnionOfTwoArrays(MyTestCase):
    """
    https://practice.geeksforgeeks.org/problems/union-of-two-arrays/0

    Given two arrays A and B. The task is to find union between these two array.
    Union of the two arrays can be defined as the set containing distinct elements from both the arrays.
    If there are repetitions, then only one occurrence of element should be printed in union.
    """

    def solution(self, A, B):
        A.sort()
        B.sort()

        a = 0
        lenA = len(A)
        b = 0
        lenB = len(B)

        def append(array, value):
            try:
                if array[-1] != value:
                    array.append(value)
            except IndexError:
                array.append(value)

        res = []
        while a < lenA and b < lenB:
            if A[a] < B[b]:
                append(res, A[a])
                a += 1
            elif A[a] > B[b]:
                append(res, B[b])
                b += 1
            else:
                append(res, A[a])
                a += 1
                b += 1

        if not (a == lenA and b == lenB):
            if a < lenA:
                remain = A
                idx = a
            else:
                remain = B
                idx = b
            for c in remain[idx:]:
                append(res, c)

        return res

    def test_one_superset(self):
        self.assertEqual(self.solution([1, 2, 3, 4, 5], [1, 2, 3]), [1, 2, 3, 4, 5])

    def test_two(self):
        self.assertEqual(self.solution([85, 25, 1, 32, 54, 6], [2]), [1, 2, 6, 25, 32, 54, 85])

    def geek_for_geeks_cases(self, a_str, b_str, result_str):
        a = [int(x) for x in a_str.split()]
        b = [int(x) for x in b_str.split()]
        self.assertEqual(self.solution(a, b), [int(x) for x in result_str.split()])

    def test_geek_for_geeks(self):
        def callback(line):
            lines.append(line)

        for i in range(1, 3):
            lines = []
            name = "test_geek_for_geeks{}".format(i)
            # print("Test data {}".format(name))
            self.get_test_data(name, callback)
            self.geek_for_geeks_cases(lines[0], lines[1], lines[2])


class MinimumCoins1(MyTestCase):
    """
    Find minimum number of coins that make a given value
    Given a value V, if we want to make change for V cents, and we have infinite supply of each of C = { C1, C2, .. , Cm} valued coins
    what is the minimum number of coins to make the change?
    """

    @staticmethod
    def update_result(result, coin):
        result[coin] = result[coin] + 1 if coin in result else 1

    def simple_solution(self, coins, value):
        def solve(coins, num_of_coins, value):
            # print(value)
            if value == 0:
                return 0
            result = sys.maxsize
            for i in range(num_of_coins):
                if coins[i] <= value:
                    # the coin value < value
                    # if use this coin, the target value is the original value minus this coin's value
                    res = solve(coins, num_of_coins, value - coins[i])

                    if (
                        res != sys.maxsize and res + 1 < result
                    ):  # do we have a solution? and if we have a solution does adding one coin (this coin) still less than current result
                        result = res + 1
            return result

        return solve(coins, len(coins), value)

    def dp_solution(self, coins, value):
        num_of_coins = len(coins)
        cache = [0] + [sys.maxsize for i in range(value)]

        for v in range(1, value + 1):
            for i in range(num_of_coins):
                if coins[i] <= v:
                    res = cache[v - coins[i]]
                    if (
                        res != sys.maxsize and res + 1 < cache[v]
                    ):  # do we have a solution? and if we have a solution does adding one coin (this coin) still less than current result
                        cache[v] = res + 1

        return cache[value]

    @staticmethod
    def gen_test_cases():
        return (
            # (sorted list coins that can be used, value V, minimum coins)
            ([1, 5, 6, 9], 0, 0),
            ([9, 6, 5, 1], 11, 2),
            ([1, 3, 6], 11, 4),
        )

    def test_simple_solution(self):
        for coins, value, result in self.gen_test_cases():
            self.assertEqual(self.simple_solution(coins, value), result)

    def test_dp_solution(self):
        for coins, value, result in self.gen_test_cases():
            self.assertEqual(self.dp_solution(coins, value), result)


class MinimumCoins2(MinimumCoins1):
    """
    https://practice.geeksforgeeks.org/problems/number-of-coins/0
    Given a value V. You have to make change for V cents, given that you have infinite supply of each of C{ C1, C2, .. , Cm} valued coins.
    Find the minimum number of coins to make the change, if not possible print "-1".
    """
    def solve(self, V, coins):
        res = self.dp_solution(coins, V)
        return res if res != sys.maxsize else -1

    def test_case1(self):
        self.assertEqual(self.solve(7, [2, 1]), 4)


if __name__ == "__main__":
    unittest.main()
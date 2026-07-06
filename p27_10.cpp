// From a sorted list of numbers and range lo-hi , find numbers missing between lo-hi

#include <iostream>
#include <cstring>
#include <vector>

#include <common.hpp>

std::vector<int> find_missing(const std::vector<int>& in, const int& lo, const int& hi)
{
    int arr1 = 0;
    int arr2 = lo;
    std::vector<int> missing;

    while (arr1 < in.size() && arr2 <= hi) {
        if (in[arr1] == arr2) {
            arr1++;
            arr2++;
        } else if (in[arr1] > arr2) {
            missing.push_back(arr2);
            arr2++;
        } else if (in[arr1] < arr2) {
            if (in[arr1] >= lo && in[arr1] <= hi) {
                missing.push_back(in[arr1]);
            }
            arr1++;
        }
    }
    return missing;
}

int main()
{
    std::vector<int> arr = { 2, 4, 5, 6, 8, 10, 14, 15 };
    int lo = 7;
    int hi = 13;

    print_vec<int>(find_missing(arr, lo, hi));

    return 0;
}

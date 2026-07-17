
// Sort Valley Shaped Array
// Prefix is sorted down
// Suffix is sorted up
// There will be non-empty prefix and suffix

#include <vector>
#include <iostream>

#include <common.hpp>

void sort_valley_array(std::vector<int>& arr)
{
    printf("To Sort: ");
    print_vec<int>(arr);

    // Find transition point
    size_t l = 0;
    size_t r = arr.size() - 1;

    std::vector<int> out(arr.size());
    int* out_ptr = &out.back();

    // we want to walk same as merge 2 sorted arrays
    while (l <= r) {
        if (arr[l] > arr[r]) {
            *out_ptr-- = arr[l];
            l++;
        } else {
            *out_ptr-- = arr[r];
            r--;
        }
    }

    printf("Sorted: ");
    print_vec<int>(out);
}

int main()
{
    std::vector<std::vector<int>> ins = { { 8, 4, 2, 6 }, { 92, 80, 70, 43, 20, 25, 46, 78 } };
    for (auto arr: ins) {
        sort_valley_array(arr);
    }
}
// Parity sort , move all even to left and all odd to right

#include <iostream>
#include <cstring>
#include <vector>

#include <common.hpp>

std::vector<int> parity_sort(std::vector<int>& in)
{
    int l = 0;
    int r = in.size() - 1;

    while (l < r) {
        if (in[l] % 2 == 0) {
            l++;
        } else {
            std::swap(in[l], in[r--]);
        }
    }

    return in;
}

int main()
{
    std::vector<int> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    print_vec<int>(parity_sort(arr));
    return 0;
}

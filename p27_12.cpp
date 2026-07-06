// In-Place duplicate removal, sorted

#include <iostream>
#include <cstring>
#include <vector>

#include <common.hpp>

std::vector<int> remove_duplicates(std::vector<int>& in)
{
    int rd = 0;
    int wr = 0;

    int prev = INT_MIN;
    while (rd < in.size()) {
        if (in[rd] != prev) {
            in[wr++] = in[rd];
        }

        prev = in[rd];
        rd++;
    }

    while (wr < in.size()) {
        in[wr++] = 0;
    }

    return in;
}

int main()
{
    std::vector<int> arr = { 1, 2, 2, 4, 5, 8, 8, 8, 10, 12 };
    print_vec<int>(remove_duplicates(arr));
    return 0;
}

// In-Place duplicate removal, sorted

#include <iostream>
#include <cstring>
#include <vector>

#include <common.hpp>

size_t quicksort_partition(std::vector<int>& in, size_t lo, size_t hi)
{
    // choose pivot
    size_t arr_sz = hi - lo;
    size_t pivot_idx = (arr_sz / 2u) + lo;
    int pivot = in[pivot_idx];

    printf("Pivot = %d\n", pivot);

    // move all the ones greater than pivot to end of arr
    int rd = lo;
    int wr = hi;

    while (rd <= wr) {
        if (in[rd] >= pivot) {
            std::swap(in[rd], in[wr--]);
        }
        rd++;
    }

    return wr;
}

std::vector<int> quick_sort(std::vector<int>& in, size_t lo, size_t hi)
{
    size_t pi = quicksort_partition(in, lo, hi);
    return {};
}

int main()
{
    {
        std::vector<int> arr = { 2, 5, 2, 5, 4, 6, 5, 7, 3, 2, 4, 3, 5 };
        size_t res = quicksort_partition(arr, 0, arr.size());
        print_vec<int>(arr);
    }
    return 0;
}

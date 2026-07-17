// Parity sort , move all even to left and all odd to right

#include <iostream>
#include <cstring>
#include <vector>

#include <common.hpp>

template<typename T>
void reverse(std::vector<T>& arr)
{
    size_t sz = arr.size();
    for (int i = 0; i < sz / 2; i++) {
        std::swap(arr[i], arr[sz - i - 1]);
    }
}

int main()
{
    std::vector<int> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    reverse<int>(arr);
    print_vec<int>(arr);
    return 0;
}

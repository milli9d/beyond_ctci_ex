#include <iostream>
#include <vector>

template <typename T>
void print_vec(const std::vector<T> &vec)
{
    for (auto i : vec)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}
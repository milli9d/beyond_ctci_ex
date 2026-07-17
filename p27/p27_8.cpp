#include <vector>
#include <algorithm>
#include <iostream>

#include <common.hpp>

// n-Way Merge Arrays
std::vector<int> n_way_merge(std::vector<std::vector<int>> arrs)
{
    std::vector<int> out = {};

    // at each iteration, look at the end of the arrays, compare these numbers, pop the max
    while (1)
    {
        std::vector<std::pair<int, std::vector<int> *>> tails = {};
        for (auto &arr : arrs)
        {
            if (!arr.empty())
            {
                tails.push_back({arr.back(), &arr});
            }
        }

        if (tails.empty())
        {
            break;
        }

        auto max_itr = std::max_element(tails.begin(), tails.end(), [](const auto &a, const auto &b)
                                        { return a.first < b.first; });
        auto arr_max = max_itr->second;

        if (out.empty() || out.back() != max_itr->first)
        {
            out.push_back(max_itr->first);
        }
        arr_max->pop_back();
    }

    std::reverse(out.begin(), out.end());

    return out;
}

int main()
{
    for (int itr = 0; itr < 10; itr++)
    {
        std::vector<std::vector<int>> arrs = {};
        size_t n_arrs = rand() % 20u;
        printf("IN n_arrs = %d\n", (int)n_arrs);
        for (int i = 0; i < (int)n_arrs; i++)
        {
            arrs.push_back(std::vector<int>());
            size_t n_elems = rand() % 10u;
            for (int j = 0; j < (int)n_elems; j++)
            {
                arrs.back().push_back(rand() % 50u);
            }
            std::sort(arrs.back().begin(), arrs.back().end());
            print_vec<int>(arrs.back());
        }

        std::vector<int> out = n_way_merge(arrs);
        printf("OUT\n");
        print_vec<int>(out);
        printf("\n\n\n");
    }

    return 0;
}
#include "catch2/catch.hpp"

#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

int increment(int *arr, size_t arr_len)
{
    for (size_t i = 0; i < arr_len; i++)
    {
        arr[i]++;
    }

    return 0;
}

#define DO_BENCHMARK_INCREMENT(title, arr_len)                       \
    {                                                                \
        vector<int> arr(arr_len);                                    \
        std::iota(arr.begin(), arr.end(), 0);                        \
                                                                     \
        int _avoid_optimization;                                     \
        BENCHMARK(title)                                             \
        {                                                            \
            _avoid_optimization = increment(arr.data(), arr.size()); \
        };                                                           \
    }

TEST_CASE("non-sgx/increment", "[benchmark]")
{
    for (size_t sz = 16; sz <= 2048; sz <<= 1)
    {
        auto title = std::to_string(sz) + "M";
        auto arr_len = (sz << 20) / sizeof(int);
        DO_BENCHMARK_INCREMENT(title.c_str(), arr_len);
    }
}
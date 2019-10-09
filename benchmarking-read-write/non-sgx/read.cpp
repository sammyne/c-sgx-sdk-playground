#include "catch2/catch.hpp"

#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

int read(const int *arr, size_t arr_len)
{
    int r = 0;
    for (size_t i = 0; i < arr_len; i++)
    {
        r = arr[i];
    }

    return r;
}

#define DO_BENCHMARK_READ(title, arr_len)                       \
    {                                                           \
        vector<int> arr(arr_len);                               \
        std::iota(arr.begin(), arr.end(), 0);                   \
                                                                \
        int _avoid_optimization;                                \
        BENCHMARK(title)                                        \
        {                                                       \
            _avoid_optimization = read(arr.data(), arr.size()); \
        };                                                      \
    }

TEST_CASE("non-sgx/read", "[benchmark]")
{
    for (size_t sz = 16; sz <= 2048; sz <<= 1)
    {
        auto title = std::to_string(sz) + "M";
        auto arr_len = (sz << 20) / sizeof(int);
        DO_BENCHMARK_READ(title.c_str(), arr_len);
    }
}
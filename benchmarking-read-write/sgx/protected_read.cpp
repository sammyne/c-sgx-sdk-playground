#include "catch2/catch.hpp"

#include <iostream>
#include <memory>
#include <vector>

#include "enclave_u.h"

#include "errors.hpp"
#include "loader.hpp"

using namespace std;

using defer = shared_ptr<void>;

#define DO_BENCHMARK_READ(title, msg_size)                                 \
    {                                                                      \
        vector<int> arr(arr_len);                                          \
        std::iota(arr.begin(), arr.end(), 0);                              \
                                                                           \
        int _avoid_optimization;                                           \
        BENCHMARK(title)                                                   \
        {                                                                  \
            auto status = ecall_read_protected(                            \
                enclave_id, &_avoid_optimization, arr.data(), arr.size()); \
            if (log_if_not_ok(status))                                     \
            {                                                              \
                return;                                                    \
            }                                                              \
        };                                                                 \
    }

TEST_CASE("sgx-protected/read", "[benchmark]")
{
    const string ENCLAVE_PATH = "../enclave/benchmarking-read-write-enclave.signed.so";

    sgx_enclave_id_t enclave_id = 0;
    {
        sgx_status_t status;

        std::tie(enclave_id, status) = load_enclave(ENCLAVE_PATH);
        if (log_if_not_ok(status))
        {
            return;
        }
    }
    defer _1(nullptr, [&](...) { unload_enclave(enclave_id); });

    for (size_t sz = 16; sz <= 2048; sz <<= 1)
    {
        auto title = std::to_string(sz) + "M";
        auto arr_len = (sz << 20) / sizeof(int);
        DO_BENCHMARK_READ(title.c_str(), arr_len);
    }
}
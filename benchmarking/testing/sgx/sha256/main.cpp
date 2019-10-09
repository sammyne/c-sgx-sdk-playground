#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include <iostream>
#include <memory>
#include <vector>

#include "enclave_u.h"

#include "errors.hpp"
#include "loader.hpp"

using namespace std;

using defer = shared_ptr<void>;

#define DO_BENCHMARK(title, msg_size)                                                            \
    {                                                                                            \
        std::vector<unsigned char> msg(msg_size, 123);                                           \
        sgx_sha256_hash_t md;                                                                    \
        BENCHMARK(title)                                                                         \
        {                                                                                        \
            sgx_status_t status_sha256;                                                          \
            auto status = ecall_sha256(enclave_id, &status_sha256, &md, msg.data(), msg.size()); \
            if (log_if_not_ok(status) || log_if_not_ok(status_sha256))                           \
            {                                                                                    \
                cout << "failed to do SHA256" << endl;                                           \
                return;                                                                          \
            }                                                                                    \
        };                                                                                       \
    }

TEST_CASE("ecall_sha256", "[benchmark]")
{
    const string ENCLAVE_PATH = "../../../enclave/enclave.signed.so";

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

    DO_BENCHMARK("msg.size=1KB", 1 << 10);
    DO_BENCHMARK("msg.size=4KB", 1 << 12);
    DO_BENCHMARK("msg.size=16KB", 1 << 14);
    DO_BENCHMARK("msg.size=64KB", 1 << 16);
}
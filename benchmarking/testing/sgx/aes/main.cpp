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

#define DO_BENCHMARK(title, msg_size)                               \
    {                                                               \
        const uint8_t key[16] = {                                   \
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,         \
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};        \
        const uint8_t iv[12] = {                                    \
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,         \
            0x01, 0x23, 0x45, 0x67};                                \
        std::vector<unsigned char> msg(msg_size, 123);              \
        vector<uint8_t> ciphertext(msg_size, 0);                    \
        uint8_t tag[16] = {0x00};                                   \
        BENCHMARK(title)                                            \
        {                                                           \
            sgx_status_t status_ret;                                \
            auto status = ecall_aes128_gcm_encrypt(                 \
                enclave_id, &status_ret,                            \
                ciphertext.data(), tag,                             \
                key, msg.data(), msg.size(), iv, sizeof(iv));       \
            if (log_if_not_ok(status) || log_if_not_ok(status_ret)) \
            {                                                       \
                cout << "failed to benchmark" << endl;              \
                return;                                             \
            }                                                       \
        };                                                          \
    }

TEST_CASE("ecall_aes128_gcm_encrypt", "[benchmark]")
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

    DO_BENCHMARK("msg.size=128B", 1 << 7);
    DO_BENCHMARK("msg.size=256B", 1 << 8);
    DO_BENCHMARK("msg.size=1KB", 1 << 10);
    DO_BENCHMARK("msg.size=4KB", 1 << 12);
    DO_BENCHMARK("msg.size=16KB", 1 << 14);
    DO_BENCHMARK("msg.size=64KB", 1 << 16);
}
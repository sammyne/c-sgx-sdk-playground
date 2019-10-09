#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <openssl/evp.h>

using namespace std;

using defer = shared_ptr<void>;

void handleErrors()
{
    cout << "error" << endl;
}

int aes128_gcm_encrypt(
    uint8_t *ciphertext, uint8_t tag[16],
    const uint8_t key[16], const uint8_t *msg, size_t msg_len, const uint8_t iv[12])
{
    /* Create and initialise the context */
    auto ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        return -1;
    }
    defer _1(nullptr, [&](...) { /* Clean up */ EVP_CIPHER_CTX_free(ctx); });

    /* Initialise the encryption operation. */
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
    {
        return -2;
    }

    /*
     * Set IV length if default 12 bytes (96 bits) is not appropriate
     */
    //if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, iv_len, NULL))
    //    handleErrors();

    /* Initialise key and IV */
    if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv))
    {
        return -3;
    }

    /*
     * Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    int len = 0;
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, msg, msg_len))
    {
        return -4;
    }
    auto ciphertext_len = len;

    /*
     * Finalise the encryption. Normally ciphertext bytes may be written at
     * this stage, but this does not occur in GCM mode
     */
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
    {
        return -5;
    }
    ciphertext_len += len;

    /* Get the tag */
    if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag))
    {
        return -6;
    }

    return ciphertext_len;
}

#define DO_BENCHMARK(title, msg_size)                        \
    {                                                        \
        const uint8_t key[16] = {                            \
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,  \
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}; \
        const uint8_t iv[12] = {                             \
            0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,  \
            0x01, 0x23, 0x45, 0x67};                         \
        std::vector<unsigned char> msg(msg_size, 123);       \
        vector<uint8_t> ciphertext(msg_size, 0);             \
        uint8_t tag[16] = {0x00};                            \
        BENCHMARK(title)                                     \
        {                                                    \
            auto status = aes128_gcm_encrypt(                \
                ciphertext.data(), tag,                      \
                key, msg.data(), msg.size(), iv);            \
            if (status < 0)                                  \
            {                                                \
                cout << "failed to benchmark" << endl;       \
                return;                                      \
            }                                                \
        };                                                   \
    }

TEST_CASE("aes128_gcm_encrypt", "[benchmark]")
{
    DO_BENCHMARK("msg.size=128B", 1 << 7);
    DO_BENCHMARK("msg.size=256B", 1 << 8);
    DO_BENCHMARK("msg.size=1KB", 1 << 10);
    DO_BENCHMARK("msg.size=4KB", 1 << 12);
    DO_BENCHMARK("msg.size=16KB", 1 << 14);
    DO_BENCHMARK("msg.size=64KB", 1 << 16);
}